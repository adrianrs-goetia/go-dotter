#pragma once

#include <core/core.hpp>
#include <core/fsm.hpp>

#include <components/dataObjects/attackInstance.h>
#include <components/dataObjects/parryInstance.hpp>

#include <godot_cpp/classes/timer.hpp>

#include <variant>

class Projectile;
class ParryInstance;

namespace ProjectileFsm {

using ExternalAction = std::variant<AttackInstance, ParryInstance>;

struct TLaunched {};
struct TParried {};
using Return = std::variant<std::monostate, TLaunched, TParried>;

struct StateContext {
	Projectile* owner{ nullptr };
};

// class BaseState : public ::State<StateContext, BaseState> {
class BaseState {
public:
	virtual const char* getName() {
		return "";
	}
	virtual bool canEnter() const {
		return true;
	};
	virtual Return enter(StateContext& context) = 0;
	virtual Return exit(StateContext& context) = 0;
	virtual Return handleExternalAction(StateContext& context, const ExternalAction& action) = 0;
	virtual Return process(StateContext& context, float delta) {
		return {};
	}
	virtual Return physicsProcess(StateContext& context, float delta) {
		return {};
	}
};

class Launched : public BaseState {
	godot::Timer* m_timer = nullptr;

public:
	Return enter(StateContext& context) override;
	Return exit(StateContext& context) override;
	Return handleExternalAction(StateContext& context, const ExternalAction& action) override;
};

class Parried : public BaseState {
public:
	Return enter(StateContext& context) override;
	Return exit(StateContext& context) override;
	Return physicsProcess(StateContext& context, float delta) override;
	Return handleExternalAction(StateContext& context, const ExternalAction& action) override;
};

// class Attacked : public BaseState {
// public:
// 	Return enter(StateContext& context) override;
// 	Return exit(StateContext& context) override;
// 	Return notification(StateContext& context, int what);
// };

// class Fsm : public ::Fsm<StateContext, BaseState, BaseState::Return> {
class Fsm {
private:
	BaseState* m_currentState = nullptr;
	Launched m_launched;
	Parried m_parried;
	// Attacked m_attacked;

public:
	void handleExternalAction(StateContext& context, const ExternalAction& action) {
		ASSERT_NOTNULL(m_currentState)
		_processState(context, m_currentState->handleExternalAction(context, action));
	}

	template <typename T>
	void setStateT(StateContext& context) {
		static_assert(std::is_base_of_v<BaseState, T>);

		if constexpr (std::is_same_v<Launched, T>) {
			m_currentState = &m_launched;
		}
		else if constexpr (std::is_same_v<T, Parried>) {
			m_currentState = &m_parried;
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
	void process(StateContext& context, float delta) {
		ASSERT_NOTNULL(m_currentState)
		_processState(context, m_currentState->process(context, delta));
	}
	void physicsProcess(StateContext& context, float delta) {
		ASSERT_NOTNULL(m_currentState)
		_processState(context, m_currentState->physicsProcess(context, delta));
	}

protected:
	void _processState(StateContext& context, Return ret) {
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

} //namespace ProjectileFsm