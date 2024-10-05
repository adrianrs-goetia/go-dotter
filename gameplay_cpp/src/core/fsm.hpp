#pragma once

/////////////////////////////////////
///	 State
/////////////////////////////////////
template <typename Context, typename Ret>
struct State {
	struct Return {
		Ret* new_state = nullptr;
	};
	virtual const char* get_name() = 0;
	virtual bool can_enter() const = 0;
	virtual Return enter(Context* context) = 0;
	virtual Return exit(Context* context) = 0;
	virtual Return process(Context* context, float delta) = 0;
	virtual Return physics_process(Context* context, float delta) = 0;
	virtual Return handle_input(Context* context, float delta) = 0;
	virtual Return deferred_actions(Context* context) = 0;
};

/////////////////////////////////////
///  Fsm
/////////////////////////////////////
template <typename Context, typename StateClass, typename StateReturn>
class Fsm {
private:
	StateClass* m_current_state = nullptr;

public:
	const char* get_current_state_name();
	void init(Context* context, StateClass* state);
	void deinit();
	void process(Context* context, float delta);
	void physics_process(Context* context, float delta);
	void handle_input(Context* context, float delta);
	void deferred_actions(Context* context);

protected:
	void _process_state(Context* context, StateReturn state_return);
};

template <typename Context, typename StateClass, typename StateReturn>
inline const char* Fsm<Context, StateClass, StateReturn>::get_current_state_name() {
	ASSERT_NOTNULL(m_current_state)
	return m_current_state->get_name();
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::init(Context* context, StateClass* state) {
	ASSERT_NOTNULL(state)
	m_current_state = state;
	_process_state(context, m_current_state->enter(context));
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::deinit() {
	m_current_state = nullptr;
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::process(Context* context, float delta) {
	ASSERT_NOTNULL(m_current_state)
	_process_state(context, m_current_state->process(context, delta));
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::physics_process(Context* context, float delta) {
	ASSERT_NOTNULL(m_current_state)
	_process_state(context, m_current_state->physics_process(context, delta));
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::handle_input(Context* context, float delta) {
	ASSERT_NOTNULL(m_current_state)
	_process_state(context, m_current_state->handle_input(context, delta));
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::deferred_actions(Context* context) {
	ASSERT_NOTNULL(m_current_state)
	_process_state(context, m_current_state->deferred_actions(context));
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::_process_state(Context* context, StateReturn state_return) {
	if (state_return.new_state && state_return.new_state->can_enter()) {
		m_current_state->exit(context);
		m_current_state = state_return.new_state;
		_process_state(context, m_current_state->enter(context));
	}
}
