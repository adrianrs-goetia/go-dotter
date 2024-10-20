#pragma once

/////////////////////////////////////
///	 State
/////////////////////////////////////
template <typename Context, typename Ret>
struct State {
	struct Return {
		Ret* new_state = nullptr;
	};
	virtual const char* getName() = 0;
	virtual bool canEnter() const = 0;
	virtual Return enter(Context& context) = 0;
	virtual Return exit(Context& context) = 0;
	virtual Return process(Context& context, float delta) = 0;
	virtual Return physicsProcess(Context& context, float delta) = 0;
	virtual Return handleInput(Context& context, float delta) = 0;
	virtual Return deferredActions(Context& context) = 0;
};

/////////////////////////////////////
///  Fsm
/////////////////////////////////////
template <typename Context, typename StateClass, typename StateReturn>
class Fsm {
private:
	StateClass* m_current_state = nullptr;

public:
	const char* getCurrentStateName();
	void init(Context& context, StateClass* state);
	void deinit();
	void process(Context& context, float delta);
	void physicsProcess(Context& context, float delta);
	void handleInput(Context& context, float delta);
	void deferredActions(Context& context);

protected:
	void _processState(Context& context, StateReturn state_return);
};

template <typename Context, typename StateClass, typename StateReturn>
inline const char* Fsm<Context, StateClass, StateReturn>::getCurrentStateName() {
	ASSERT_NOTNULL(m_current_state)
	return m_current_state->get_name();
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::init(Context& context, StateClass* state) {
	ASSERT_NOTNULL(state)
	m_current_state = state;
	_processState(context, m_current_state->enter(context));
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::deinit() {
	m_current_state = nullptr;
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::process(Context& context, float delta) {
	ASSERT_NOTNULL(m_current_state)
	_processState(context, m_current_state->process(context, delta));
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::physicsProcess(Context& context, float delta) {
	ASSERT_NOTNULL(m_current_state)
	_processState(context, m_current_state->physicsProcess(context, delta));
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::handleInput(Context& context, float delta) {
	ASSERT_NOTNULL(m_current_state)
	_processState(context, m_current_state->handleInput(context, delta));
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::deferredActions(Context& context) {
	ASSERT_NOTNULL(m_current_state)
	_processState(context, m_current_state->deferredActions(context));
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::_processState(Context& context, StateReturn state_return) {
	if (state_return.new_state && state_return.new_state->canEnter()) {
		m_current_state->exit(context);
		m_current_state = state_return.new_state;
		_processState(context, m_current_state->enter(context));
	}
}
