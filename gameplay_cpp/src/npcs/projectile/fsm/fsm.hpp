#pragma once

#include "typedefs.hpp"

#include "states/launched.hpp"
#include "states/parried.hpp"
#include "states/postParryLaunch.hpp"

namespace fsm::projectile {

class Fsm {
	Context m_context;
	BaseState* m_currentState = nullptr;

	Launched m_launched;
	Parried m_parried;
	PostParryLaunch m_postParryLaunch;

public:
	Fsm(Context context)
		: m_context(context) {}

	TState getCurrentState() const {
		ASSERT_NOTNULL(m_currentState)
		return m_currentState->getType();
	}

	void handleExternalAction(const ExternalAction& action) {
		ASSERT_NOTNULL(m_currentState)
		_processState(m_currentState->handleExternalAction(m_context, action));
	}

	template <typename T>
	void init() {
		if constexpr (std::is_same_v<T, TLaunched>) {
			m_currentState = &m_launched;
		}
		else if constexpr (std::is_same_v<T, TParried>) {
			m_currentState = &m_parried;
		}
		else if constexpr (std::is_same_v<T, TPostParryLaunched>) {
			m_currentState = &m_postParryLaunch;
		}
		else {
			ASSERT(false)
		}
		ASSERT_NOTNULL(m_currentState)
		_processState(m_currentState->enter(m_context));
	}
	void deinit() {
		m_currentState = nullptr;
	}
	void process(float delta) {
		ASSERT_NOTNULL(m_currentState)
		_processState(m_currentState->process(m_context, delta));
	}
	void physicsProcess(float delta) {
		ASSERT_NOTNULL(m_currentState)
		_processState(m_currentState->physicsProcess(m_context, delta));
	}

protected:
	void _processState(TState ret) {
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
			m_currentState->exit(m_context);
			m_currentState = newState;
			_processState(m_currentState->enter(m_context));
		}
	}
};

} //namespace fsm::projectile