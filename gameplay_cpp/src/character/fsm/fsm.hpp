#pragma once

#include "typedefs.hpp"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/physics_direct_body_state3d.hpp>

// #include "states/attack.hpp"
// #include "states/grapplelaunch.hpp"
#include "states/inair.hpp"
#include "states/onground.hpp"
// #include "states/parryjump.hpp"
// #include "states/parrypost.hpp"
// #include "states/parrypre.hpp"
// #include "states/pregrapplelaunch.hpp"

namespace fsm::player {

class Fsm {
	Context _context;
	BaseState* _currentState = nullptr;

	// AttackState _attackState;
	OnGroundState _onGroundState;
	// GrappleLaunchState _grappleLaunchState;
	InAirState _inAirState;
	// ParryPreState _parryState;
	// ParryPostState _parryPostState;
	// ParryJumpState _parryJumpState;
	// PreGrappleLaunchState _preGrappleLaunchState;

public:
	Fsm(Context context)
		: _context(context) {}

	Context& getContext() {
		return _context;
	}

	void init(TState state) {
		_currentState = &_onGroundState;

		ASSERTNN(_currentState)
		_context.states->push(state);
		_processState(_currentState->enter(_context));
	}

	TState getCurrentState() const {
		ASSERTNN(_currentState)
		return _currentState->getType();
	}

	void deinit() {
		_currentState = nullptr;
	}

	void integrateForces(godot::PhysicsDirectBodyState3D* state) {
		ASSERTNN(_currentState)
		_processState(_currentState->integrateForces(_context, state));
	}

	void handleInput(float delta) {
		ASSERTNN(_currentState)
		_processState(_currentState->handleInput(_context, delta));
	}

private:
	void _processState(TState state) {
		if (std::holds_alternative<std::monostate>(state)) {
			return;
		}

		BaseState* newState = nullptr;

		std::visit(
			overloaded{
				// [&](TAttackState) { newState = &_attackState; },
				[&](TOnGroundState) { newState = &_onGroundState; },
				// [&](TGrappleLaunchState) { newState = &_grappleLaunchState; },
				[&](TInAirState) { newState = &_inAirState; },
				// [&](TParryPreState) { newState = &_parryState; },
				// [&](TParryPostState) { newState = &_parryPostState; },
				// [&](TParryJumpState) { newState = &_parryJumpState; },
				// [&](TPreGrappleLaunchState) { newState = &_preGrappleLaunchState; },
				[&](auto) { ASSERT(false, "Unexpected state during Fsm process state"); },
			},
			state);

		if (newState && newState->canEnter()) {
			ASSERTNN(_currentState)
			_currentState->exit(_context);
			_context.states->push(state);
			_currentState = newState;
			_processState(_currentState->enter(_context));
		}
	}
};

} //namespace fsm::player