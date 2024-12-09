#pragma once

#include <core/core.hpp>
#include <core/fsm.hpp>
#include <godot_cpp/classes/timer.hpp>

class Projectile;

namespace ProjectileFsm {

struct StateContext {
	Projectile* owner{ nullptr };
};

class BaseState : public ::State<StateContext, BaseState> {
public:
	virtual const char* getName() {
		return "";
	}
	virtual bool canEnter() const {
		return true;
	};
	virtual Return enter(StateContext& context) = 0;
	virtual Return exit(StateContext& context) = 0;
	virtual Return process(StateContext& context, float delta) {
		return {};
	}
	virtual Return physicsProcess(StateContext& context, float delta) {
		return {};
	}

	virtual Return notification(StateContext& context, int what) = 0;
};

class Launched : public BaseState {
	godot::Timer* m_timer = nullptr;

public:
	Return enter(StateContext& context) override;
	Return exit(StateContext& context) override;
	Return notification(StateContext& context, int what);
};
// class Parried : public BaseState {
// public:
// 	Return enter(StateContext& context) override;
// 	Return exit(StateContext& context) override;
// 	Return notification(StateContext& context, int what);
// };
// class Attacked : public BaseState {
// public:
// 	Return enter(StateContext& context) override;
// 	Return exit(StateContext& context) override;
// 	Return notification(StateContext& context, int what);
// };

class Fsm : public ::Fsm<StateContext, BaseState, BaseState::Return> {
private:
	Launched m_launched;
	// Parried m_parried;
	// Attacked m_attacked;

public:
	void notification(StateContext& context, int what) {
		ASSERT_NOTNULL(m_currentState)
		_processState(context, m_currentState->notification(context, what));
	}

	template <typename T>
	void setStateT(StateContext& context) {
		static_assert(std::is_base_of_v<BaseState, T>);

		if constexpr (std::is_same_v<Launched, T>) {
			m_currentState = &m_launched;
		}
		// else if constexpr (std::is_same_v<T, Parried>) {
		// 	m_currentState = &m_parried;
		// }
		// else if constexpr (std::is_same_v<T, Attacked>) {
		// 	m_currentState = &m_attacked;
		// }
		init(context, m_currentState);
	}
};

} //namespace ProjectileFsm