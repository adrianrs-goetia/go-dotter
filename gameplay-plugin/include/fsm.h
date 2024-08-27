#pragma once

template <typename Context, typename Ret>
struct State {
	struct Return {
		Ret* new_state = nullptr;
	};
	virtual const char* get_state_name() = 0;
	virtual bool can_enter() const = 0;
	virtual Return enter(Context* context) = 0;
	virtual Return exit(Context* context) = 0;
	virtual Return process(Context* context, float delta) = 0;
	virtual Return physics_process(Context* context, float delta) = 0;
	virtual Return handle_input(Context* context) = 0;
	virtual Return deferred_actions(Context* context) = 0;
};

template <typename Context, typename StateType, typename StateReturn>
class Fsm {
private:
	StateType* m_current_state = nullptr;

public:
	const char* get_current_state_name();
	virtual void init(StateType* state) = 0;
	virtual void deinit() = 0;
	virtual void process(Context* context, float delta) = 0;
	virtual void physics_process(Context* context, float delta) = 0;
	virtual void handle_input(Context* context) = 0;
	virtual void deferred_actions(Context* context) = 0;

protected:
	void _process_state(Context* context, StateReturn state_return);
};

struct PlayerContext {
	float x, y, z; //example
	int a, b, c;
};
struct PlayerState : public State<PlayerContext, PlayerState> {
	virtual Return enter(PlayerContext* context) override { return {}; }
};
class PlayerStateFsm : public Fsm<PlayerContext, PlayerState, PlayerState::Return> {};

void test(float delta) {
	PlayerContext context;
	PlayerState state;
	auto ret = state.enter(&context);

	PlayerStateFsm fsm;
	fsm.init(&state);
	fsm.process(&context, delta);
}
