
#ifndef GD_CHARACTER_PLAYERFSM_PLUGIN_STEIKEMANNGAMEPLAY_H
#define GD_CHARACTER_PLAYERFSM_PLUGIN_STEIKEMANNGAMEPLAY_H

#include <core/core.h>
#include <core/godotincludes.h>

using namespace godot;

class PlayerState;
class PlayerFSM;

// Data som en playerstate alltid er interessert i
//  Forer inn en ny struct som dette i hver _physics_process
struct StatePhysicsContext {
	bool is_on_ground;
	Vector3 position;
	Vector3 velocity;
};

// Data FSM and State receives from an owner regarding _unhandled_input
struct StateInputContext {
	Vector2 input_direction;
	InputAction input_action;
	InputAction last_valid_input_action;
};
struct StateContext {
	StatePhysicsContext physics;
	StateInputContext input;
};

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
	bool m_guarantee_one_frame = false; // guarantee that frame is processed for a single physics process
										// TODO: deferr to end of frame somehow in PlayerNode? Not just _physics_process

public:
	friend class PlayerFSM;

	PlayerState(bool one_frame);
	PlayerState();

	virtual StateReturn enter_state(StateContext* context);
	virtual void exit_state(StateContext* context);

	virtual StateReturn process(StateContext* context, real_t delta) = 0;
	virtual StateReturn physics_process(StateContext* context, real_t delta) = 0;
	virtual StateReturn handle_input(StateContext* context, real_t delta) = 0;
	virtual const char* get_class_name() = 0;

	virtual void deferred_actions(StateContext* context) { m_guarantee_one_frame = false; }

// Macros to be implemented on each class inheriting this
#define PLAYER_STATE_IMPL(CLASSNAME)                                                                                   \
	typedef PlayerState Super;                                                                                         \
	CLASSNAME(bool one_frame) : PlayerState(one_frame) {}                                                              \
	CLASSNAME() : PlayerState() {}                                                                                     \
                                                                                                                       \
	template <typename T> const char* _get_class_name() { return typeid(T).name(); }                                   \
	virtual const char* get_class_name() override { return _get_class_name<CLASSNAME>(); }
};

// statuses: Free, HoldingEnemy
// states: OnGround, Falling, HoldingEnemy_OnGround, HoldingEnemy_Falling
class PlayerFSM {
public:
	PlayerState* m_current_state = nullptr;

public:
	virtual void _exit_tree();

	void process(StateContext* context, real_t delta);
	void physics_process(StateContext* context, real_t delta);
	void handle_input(StateContext* context, real_t delta);
	void deferred_actions(StateContext* context); // After physics_process and handle input

	template <typename T> void force_set_state(StateContext* context) {
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

#endif // GD_CHARACTER_PLAYERFSM_PLUGIN_STEIKEMANNGAMEPLAY_H