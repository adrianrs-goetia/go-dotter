
#ifndef GD_CHARACTER_PLAYERFSM_PLUGIN_GAMEPLAY_H
#define GD_CHARACTER_PLAYERFSM_PLUGIN_GAMEPLAY_H

#include <functional>

#include <core/core.h>
#include <core/fsm.hpp>

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class PlayerState;
class PlayerFSM;
class GrappleComponent;
class ParryInstigatorComponent;
class InputComponent;

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
struct StateGrappleContext {
	GrappleComponent* instigator = nullptr;
	GrappleComponent* target = nullptr;
};
// struct StateParryContext {
// 	float detectionradius{};
// 	std::function<TypedArray<RID>()> get_rid = nullptr;
// 	std::function<Ref<World3D>()> get_world = nullptr;
// 	std::function<Ref<Shape3D>()> get_shape = nullptr;

// 	TypedArray<Vector3> get_parry_physics_query(const Vector3& gravity_center) {
// 		ASSERT(get_rid != nullptr, "")
// 		ASSERT(get_world != nullptr, "")
// 		ASSERT(get_shape != nullptr, "")

// 		Ref<PhysicsShapeQueryParameters3D> query;
// 		query.instantiate();
// 		query->set_shape(get_shape());
// 		query->set_transform(Transform3D(Basis(), gravity_center));
// 		query->set_collide_with_areas(true);
// 		query->set_collide_with_bodies(true);
// 		query->set_exclude(get_rid());
// 		PhysicsDirectSpaceState3D* space_state = get_world()->get_direct_space_state();
// 		ASSERT(space_state != nullptr, "")
// 		return space_state->collide_shape(query);
// 	}
// };

struct StateContext {
	StateGrappleContext grapple;
	InputComponent* input = nullptr;
	// StateParryContext parry;
	ParryInstigatorComponent* parry = nullptr; // ParryComponent == Players ParryDetection/ParryLogic
	StatePhysicsContext physics;
};

namespace Compiletime {
	constexpr int sizeof_physicscontext = sizeof(StatePhysicsContext);
	// constexpr int sizeof_inputcontext = sizeof(InputComponent);
	constexpr int sizeof_grapplecontext = sizeof(StateGrappleContext);
	// constexpr int sizeof_parrycontext = sizeof(StateParryContext);
	constexpr int sizeof_statecontext = sizeof(StateContext);

	// static_assert(sizeof(StatePhysicsContext) == 28);
	// static_assert(sizeof(StateInputContext) == 56);
	// static_assert(sizeof(StateContext) == 136);
} //namespace Compiletime

class PlayerState : public State<StateContext, PlayerState> {
	// BUG: regarding guarantee one frame processing?

public:
	// default overrides for PlayerState
	virtual bool canEnter() const override { return true; }
	virtual Return enter(StateContext& context) override {
		LOG(DEBUG, "Entering state ", get_name())
		return {};
	}
	virtual Return exit(StateContext& context) override { return {}; }
	virtual Return process(StateContext& context, float delta) override { return {}; }
	virtual Return physicsProcess(StateContext& context, float delta) override { return {}; }
	virtual Return handleInput(StateContext& context, float delta) override { return {}; }
	virtual Return deferredActions(StateContext& context) override { return {}; }

#define PLAYER_STATE_IMPL(CLASSNAME)                                                                                   \
	typedef PlayerState Super;                                                                                         \
	virtual const char* get_name() override{ return #CLASSNAME;                                                        \
	}
};

class PlayerFSM : public Fsm<StateContext, PlayerState, PlayerState::Return> {};

#endif // GD_CHARACTER_PLAYERFSM_PLUGIN_GAMEPLAY_H