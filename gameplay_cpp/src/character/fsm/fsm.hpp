#pragma once

#include "typedefs.hpp"

#include <godot_cpp/classes/node3d.hpp>

#include "states/attack.hpp"
#include "states/grapplelaunch.hpp"
#include "states/inair.hpp"
#include "states/onground.hpp"
#include "states/parryjump.hpp"
#include "states/parrypost.hpp"
#include "states/parrypre.hpp"
#include "states/pregrapplelaunch.hpp"

namespace fsm::player {

class Fsm {
	Context _context;
	BaseState* _currentState = nullptr;

	AttackState _attackState;
	OnGroundState _onGroundState;
	GrappleLaunchState _grappleLaunchState;
	InAirState _inAirState;
	ParryPreState _parryState;
	ParryPostState _parryPostState;
	ParryJumpState _parryJumpState;
	PreGrappleLaunchState _preGrappleLaunchState;

public:
	Fsm(Context context)
		: _context(context) {}

	Context& getContext() {
		return _context;
	}

	void init(TState state) {
		std::visit(
			overloaded{
				[this](TAttackState) { _currentState = &_attackState; },
				[this](TOnGroundState) { _currentState = &_onGroundState; },
				[this](TGrappleLaunchState) { _currentState = &_grappleLaunchState; },
				[this](TInAirState) { _currentState = &_inAirState; },
				[this](TParryPreState) { _currentState = &_parryState; },
				[this](TParryPostState) { _currentState = &_parryPostState; },
				[this](TParryJumpState) { _currentState = &_parryJumpState; },
				[this](TPreGrappleLaunchState) { _currentState = &_preGrappleLaunchState; },
				[this](std::monostate) { ASSERT(false); },
			},
			state);

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

	void process(float delta) {
		ASSERTNN(_currentState)
		_processState(_currentState->process(_context, delta));
	}

	void physicsProcess(float delta) {
		ASSERTNN(_currentState)
		_processState(_currentState->physicsProcess(_context, delta));
	}

	void handleInput(float delta) {
		ASSERTNN(_currentState)
		_processState(_currentState->handleInput(_context, delta));
	}

	void deferredPhysicsProcess(float delta) {
		ASSERTNN(_currentState)
		_processState(_currentState->deferredPhysicsProcess(_context, delta));
	}

private:
	void _processState(TState state) {
		if (std::holds_alternative<std::monostate>(state)) {
			return;
		}

		BaseState* newState = nullptr;

		std::visit(
			overloaded{
				[&](TAttackState) { newState = &_attackState; },
				[&](TOnGroundState) { newState = &_onGroundState; },
				[&](TGrappleLaunchState) { newState = &_grappleLaunchState; },
				[&](TInAirState) { newState = &_inAirState; },
				[&](TParryPreState) { newState = &_parryState; },
				[&](TParryPostState) { newState = &_parryPostState; },
				[&](TParryJumpState) { newState = &_parryJumpState; },
				[&](TPreGrappleLaunchState) { newState = &_preGrappleLaunchState; },
				[&](std::monostate) { ASSERT(false); },
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