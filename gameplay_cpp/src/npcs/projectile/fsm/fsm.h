#pragma once

#include "typedef.h"

#include "states/launched.h"
#include "states/parried.h"

namespace fsm::projectile {

// class Attacked : public BaseState {
// public:
// 	StateT enter(Context& context) override;
// 	StateT exit(Context& context) override;
// 	StateT notification(Context& context, int what);
// };

// class Fsm : public ::Fsm<Context, BaseState, BaseState::Return> {
class Fsm {
private:
	BaseState* m_currentState = nullptr;
	Launched m_launched;
	Parried m_parried;
	// Attacked m_attacked;

public:
	TState getCurrentState() const {
		ASSERT_NOTNULL(m_currentState)
		return m_currentState->getType();
	}

	void handleExternalAction(Context& context, const ExternalAction& action) {
		ASSERT_NOTNULL(m_currentState)
		_processState(context, m_currentState->handleExternalAction(context, action));
	}

	template <typename T>
	void setStateT(Context& context) {
		// static_assert(isTypeInVariantV<T, TState>);

		if constexpr (std::is_same_v<T, TLaunched>) {
			m_currentState = &m_launched;
		}
		else if constexpr (std::is_same_v<T, TParried>) {
			m_currentState = &m_parried;
		}
		else {
			ASSERT(false)
		}
		// else if constexpr (std::is_same_v<T, Attacked>) {
		// 	m_currentState = &m_attacked;
		// }
		ASSERT_NOTNULL(m_currentState)
		_processState(context, m_currentState->enter(context));
	}
	void deinit() {
		m_currentState = nullptr;
	}
	void process(Context& context, float delta) {
		ASSERT_NOTNULL(m_currentState)
		_processState(context, m_currentState->process(context, delta));
	}
	void physicsProcess(Context& context, float delta) {
		ASSERT_NOTNULL(m_currentState)
		_processState(context, m_currentState->physicsProcess(context, delta));
	}

protected:
	void _processState(Context& context, TState ret) {
		if (std::holds_alternative<std::monostate>(ret)) {
			return;
		}

		BaseState* newState = nullptr;
		if (std::holds_alternative<TLaunched>(ret)) {
			newState = &m_launched;
		}
		else if (std::holds_alternative<TParried>(ret)) {
			newState = &m_parried;
		}
		if (newState && newState->canEnter()) {
			ASSERT_NOTNULL(m_currentState)
			m_currentState->exit(context);
			m_currentState = newState;
			_processState(context, m_currentState->enter(context));
		}
	}
};

} //namespace fsm::projectile