#pragma once

/////////////////////////////////////
///	 State
/////////////////////////////////////
template <typename Context, typename Ret>
struct State {
	virtual ~State(){}
	struct Return {
		Ret* new_state = nullptr;
	};
	virtual const char* getName() = 0;
	virtual bool canEnter() const = 0;
	virtual Return enter(Context& context) = 0;
	virtual Return exit(Context& context) = 0;
	virtual Return process(Context& context, float delta) = 0;
	virtual Return physicsProcess(Context& context, float delta) = 0;
};

/////////////////////////////////////
///  Fsm
/////////////////////////////////////
template <typename Context, typename StateClass, typename StateReturn>
class Fsm {
protected:
	StateClass* m_currentState = nullptr;

public:
	const char* getCurrentStateName();
	void init(Context& context, StateClass* state);
	void deinit();
	void process(Context& context, float delta);
	void physicsProcess(Context& context, float delta);

protected:
	void _processState(Context& context, StateReturn state_return);
};

template <typename Context, typename StateClass, typename StateReturn>
inline const char* Fsm<Context, StateClass, StateReturn>::getCurrentStateName() {
	ASSERT_NOTNULL(m_currentState)
	return m_currentState->get_name();
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::init(Context& context, StateClass* state) {
	ASSERT_NOTNULL(state)
	m_currentState = state;
	_processState(context, m_currentState->enter(context));
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::deinit() {
	m_currentState = nullptr;
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::process(Context& context, float delta) {
	ASSERT_NOTNULL(m_currentState)
	_processState(context, m_currentState->process(context, delta));
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::physicsProcess(Context& context, float delta) {
	ASSERT_NOTNULL(m_currentState)
	_processState(context, m_currentState->physicsProcess(context, delta));
}

template <typename Context, typename StateClass, typename StateReturn>
inline void Fsm<Context, StateClass, StateReturn>::_processState(Context& context, StateReturn state_return) {
	if (state_return.new_state && state_return.new_state->canEnter()) {
		m_currentState->exit(context);
		m_currentState = state_return.new_state;
		_processState(context, m_currentState->enter(context));
	}
}

