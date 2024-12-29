#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <debugdraw3d/api.h>
#include <managers/inputManager.h>
#include <configparams.hpp>

#include <components/animation.hpp>
#include <components/parryInstigator.hpp>
#include <events/parry.hpp>

#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>

namespace fsm::player {

class ParryPreState : public BaseState {
	TYPE(ParryPreState)

	ConfigParam::Player::Parry::Pre param;

private:
	Timestamp m_enterTimestamp;
	Timestamp m_exitTimestamp;

	struct {
		godot::Vector3 direction;
		float speed;
	} slide;

public:
	TState getType() const override {
		return TParryPreState();
	}

	bool canEnter() const override {
		const bool canEnter = !m_exitTimestamp.timestampWithinTimeframe(param.cooldown());
		if (!canEnter) {
			LOG(DEBUG, "Tried entering parryState before cooldown was ready")
		}
		return canEnter;
	}

	TState enter(Context& context) override {
		context.weapon->enableTrail(); 
		auto previousState = context.states->get(1);
		std::visit(
			overloaded{
				[&](TOnGroundState) { context.anim->doParry(true, true); },
				[&](TInAirState) { context.anim->doParry(true, false); }, //dont animate legs
				[&](auto state)
				{
					LOG(DEBUG, "Entered parry from unexpected state ", stateName(state));
					ASSERT(false);
				},
			},
			previousState);
		m_enterTimestamp.setTimestamp();

		auto inputDir = utils::getInputOrForward(context);
		if (utils::isOnFloor(*context.physics.state)) {
			slide.speed = param.onground.slide.basespeed();
			context.physics.movement
				= inputDir * (context.input->isInputActive() ? slide.speed : param.onground.slide.inactivebasespeed());
		}
		else {
			context.physics.movement *= 0;
		}
		context.anim->setRootTowardsVector(inputDir);
		return {};
	}

	TState exit(Context& context) override {
		context.weapon->disableTrail(); 
		context.anim->dontParry();
		m_exitTimestamp.setTimestamp();
		return {};
	}

	TState integrateForces(Context& context, godot::PhysicsDirectBodyState3D* state) override {
		const float delta = state->get_step();
		const bool isOnFloor = utils::isOnFloor(*state);
		const auto parryDirection = utils::getInputOrForward(context);

		auto& move = context.physics.movement;
		if (isOnFloor) {
			auto desiredMove = utils::lerpVectorSpeedToInput(context,
				move,
				(context.input->isInputActive() ? param.onground.slide.basespeed()
												: param.onground.slide.inactivebasespeed()),
				param.onground.slide.acceleration(),
				param.onground.slide.acceleration(),
				delta);

			move = desiredMove;
		}
		else {
			move *= 0;
		}
		state->set_linear_velocity(move);

		const auto length = isOnFloor ? param.onground.length() : param.inair.length();
		const auto lift = isOnFloor ? param.onground.lift() : param.inair.lift();
		// Check if successful parry
		if (const auto pi = context.parry->activateParry(EventParry::Params{ parryDirection, length, lift })) {
			context.gui->increment();

			if (!isOnFloor) {
				auto impulse = pi.value().params.direction * param.inair.impulse.xz();
				impulse.y = param.inair.impulse.y();
				context.physics.movement = impulse;
			}

			// Play effects
			context.audioVisual.audio->play();
			context.audioVisual.particles->set_global_position(pi->targetPosition);
			context.audioVisual.particles->set_global_basis(createBasisFromDirection(pi->getDirectionToTarget3D()));
			context.audioVisual.particles->restart();

			return TParryPostState();
		}

		if (!m_enterTimestamp.timestampWithinTimeframe(param.timeout())) {
			// Enter on ground by default, should discern if in air or onGround?
			return isOnFloor ? TState(TOnGroundState()) : TState(TInAirState());
		}

		return {};
	}

	TState handleInput(Context& context, float delta) override {
		// if (context.input->isActionPressed(EInputAction::JUMP)) {
		// 	return TInAirState();
		// }
		return {};
	}
};

} //namespace fsm::player
