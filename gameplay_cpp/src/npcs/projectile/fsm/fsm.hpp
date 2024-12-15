#pragma once

#include "typedefs.hpp"

#include "states/launched.hpp"
#include "states/parried.hpp"
#include "states/parryFreeze.hpp"
#include "states/postParryLaunch.hpp"

namespace fsm::projectile {

class Fsm {
	Context m_context;
	BaseState* m_currentState = nullptr;

	Launched m_launched;
	Parried m_parried;
	ParryFreeze m_parryFreeze;
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

	void init(TState state) {
		std::visit(
			overloaded{
				[](std::monostate) { ASSERT(false) },
				[this](TLaunched) { m_currentState = &m_launched; },
				[this](TParried) { m_currentState = &m_parried; },
				[this](TPostParryLaunched) { m_currentState = &m_postParryLaunch; },
				[this](TParryFreeze) { m_currentState = &m_parryFreeze; },
			},
			state);

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
		std::visit(
			overloaded{
				[](std::monostate) { ASSERT(false) },
				[&](TLaunched) { newState = &m_launched; },
				[&](TParried) { newState = &m_parried; },
				[&](TPostParryLaunched) { newState = &m_postParryLaunch; },
				[&](TParryFreeze) { newState = &m_parryFreeze; },
			},
			ret);

		if (newState && newState->canEnter()) {
			ASSERT_NOTNULL(m_currentState)
			m_currentState->exit(m_context);
			m_currentState = newState;
			_processState(m_currentState->enter(m_context));
		}
	}
};

} //namespace fsm::projectile