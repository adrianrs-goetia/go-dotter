
#ifndef GD_CHARACTER_PLAYERFSM_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_PLAYERFSM_PLUGIN_GAMEPLAY_H

#include <functional>

#include <core/core.h>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/world3d.hpp>

using namespace godot;

class PlayerState;
class PlayerFSM;

constexpr float PLAYER_CHARACTER_HEIGHT = 2.0f;
constexpr float PLAYER_CHARACTER_HALFHEIGHT = PLAYER_CHARACTER_HEIGHT / 2.f;

// Data som en playerstate alltid er interessert i
//  Forer inn en ny struct som dette i hver _physics_process
struct StatePhysicsContext {
	bool is_on_ground;
	Vector3 position;
	Vector3 velocity;

	Vector3 get_gravity_center() const {
		return Vector3(position.x, position.y + PLAYER_CHARACTER_HALFHEIGHT, position.z);
	}
};
// Data FSM and State receives from an owner regarding _input
struct StateInputContext {
	EInputMode mode;

	Vector2 movedir;
	Vector2 movedir_rotated;
	Vector2 motion;
	Vector2 camera2ddir;

	InputAction input_action;
	InputAction last_valid_input_action;
};
struct StateGrappleContext {
	Node3D* target = nullptr;
	Vector3 target_position;
};
struct StateParryContext {
	float detectionradius{};
	std::function<TypedArray<RID>()> get_rid = nullptr;
	std::function<Ref<World3D>()> get_world = nullptr;
	std::function<Ref<Shape3D>()> get_shape = nullptr;

	TypedArray<Vector3> get_parry_physics_query(const Vector3& gravity_center) {
		ASSERT(get_rid != nullptr, "")
		ASSERT(get_world != nullptr, "")
		ASSERT(get_shape != nullptr, "")

		Ref<PhysicsShapeQueryParameters3D> query;
		query.instantiate();
		query->set_shape(get_shape());
		query->set_transform(Transform3D(Basis(), gravity_center));
		query->set_collide_with_areas(true);
		query->set_collide_with_bodies(true);
		query->set_exclude(get_rid());
		PhysicsDirectSpaceState3D* space_state = get_world()->get_direct_space_state();
		ASSERT(space_state != nullptr, "")
		return space_state->collide_shape(query);
	}
};

struct StateContext {
	StatePhysicsContext physics;
	StateInputContext input;
	StateGrappleContext grapple;
	StateParryContext parry;
};

namespace Compiletime {
	// constexpr int sizeof_statephysicscontext = sizeof(StatePhysicsContext);
	// constexpr int sizeof_stateinputcontext = sizeof(StateInputContext);
	constexpr int sizeof_statecontext = sizeof(StateContext);

	// static_assert(sizeof(StatePhysicsContext) == 28);
	// static_assert(sizeof(StateInputContext) == 56);
	// static_assert(sizeof(StateContext) == 136);
} //namespace Compiletime

enum class EStateReturn : uint8_t {
	CONTINUE,
	NEW_STATE,
};
struct StateReturn {
	EStateReturn ret_enum = EStateReturn::CONTINUE;
	PlayerState* new_state = nullptr;
};

class PlayerState {
public:
	// guarantee that frame is processed for a single physics process
	// TODO: deferr to end of frame somehow in PlayerNode? Not just _physics_process
	bool m_guarantee_one_frame_processing = false;

	friend class PlayerFSM;

public:
	void init(bool guarantee_one_frame_processing);
	virtual StateReturn enter_state(StateContext* context);
	virtual void exit_state(StateContext* context);

	virtual StateReturn process(StateContext* context, float delta) = 0;
	virtual StateReturn physics_process(StateContext* context, float delta) = 0;
	virtual StateReturn handle_input(StateContext* context, float delta) = 0;
	virtual const char* get_class_name() = 0;

	virtual void deferred_actions(StateContext* context) { m_guarantee_one_frame_processing = false; }

// Macros to be implemented on each class inheriting this
#define PLAYER_STATE_IMPL(CLASSNAME)                                                                                   \
	typedef PlayerState Super;                                                                                         \
	virtual const char* get_class_name() override { return #CLASSNAME; }
	// CLASSNAME() : PlayerState() {}
	// CLASSNAME(bool one_frame) : PlayerState(one_frame) {} \

};

class PlayerFSM {
public:
	PlayerState* m_current_state = nullptr;

public:
	virtual void _exit_tree();

	void process(StateContext* context, float delta);
	void physics_process(StateContext* context, float delta);
	void handle_input(StateContext* context, float delta);
	void deferred_actions(StateContext* context); // After physics_process and handle input

	template <typename T>
	void force_set_state(StateContext* context) {
		static_assert(std::is_base_of_v<PlayerState, T>, "FSM requires class State as base");
		if (m_current_state) {
			m_current_state->exit_state(context);
			delete m_current_state;
			m_current_state = nullptr;
		}
		m_current_state = new T();
		ASSERT(m_current_state != nullptr, "");
		m_current_state->enter_state(context);
	}

protected:
	void _process_state(StateContext* context, StateReturn state_return);
};

#endif // GD_CHARACTER_PLAYERFSM_PLUGIN_GAMEPLAY_H