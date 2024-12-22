#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <configHandler.h>
#include <debugdraw3d/api.h>
#include <managers/inputManager.h>

#include <components/animation.hpp>
#include <components/parryInstigator.hpp>
#include <events/parry.hpp>

#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>

#ifdef CONFIG_PREFIX
#undef CONFIG_PREFIX
#endif
#define CONFIG_PREFIX "player", "parry", "pre"

namespace fsm::player {

class ParryPreState : public BaseState {
	TYPE(ParryPreState)

private:
	Timestamp m_enterTimestamp;
	Timestamp m_exitTimestamp;

public:
	TState getType() const override {
		return TParryPreState();
	}

	bool canEnter() const override {
		const bool canEnter = !m_exitTimestamp.timestampWithinTimeframe(GETPARAM_D("cooldown"));
		if (!canEnter) {
			LOG(DEBUG, "Tried entering parryState before cooldown was ready")
		}
		return canEnter;
	}

	TState enter(Context& context) override {
		auto previousState = context.states->get(1);
		std::visit(
			overloaded{
				[&](TOnGroundState) { context.anim->doParry(true, true); }, 
				[&](TInAirState)    { context.anim->doParry(true, false); }, //dont animate legs
				[&](auto state) { LOG(DEBUG, "Entered parry from unexpected state ", stateName(state)); ASSERT(false); },
			},
			previousState);
		m_enterTimestamp.setTimestamp();
		context.physics.velocity = godot::Vector3(); // zero out velocity while in
		return {};
	}

	TState exit(Context& context) override {
		context.anim->dontParry();
		m_exitTimestamp.setTimestamp();
		return {};
	}

	TState process(Context& context, float delta) override {
		return {};
	}

	TState physicsProcess(Context& context, float delta) override {
		context.physics.velocity = Vector3();

		if (!m_enterTimestamp.timestampWithinTimeframe(GETPARAM_D("stateTime"))) {
			// Enter on ground by default, should discern if in air or onGround?
			return TOnGroundState();
		}

		godot::Vector3 parryDirection = context.input->getInputRelative3d();
		if (parryDirection.length_squared() < 0.2f) {
			parryDirection = context.anim->m_animRoot->get_global_basis().get_column(2);
		}

		if (const auto pi = context.parry->activateParry(
				EventParry::Params{ parryDirection, GETPARAM_F("length"), GETPARAM_F("lift") })) {
			// Play effects
			context.audioVisual.audio->play();
			context.audioVisual.particles->set_global_position(pi->targetPosition);
			context.audioVisual.particles->set_global_basis(createBasisFromDirection(pi->getDirectionToTarget3D()));
			context.audioVisual.particles->restart();

			return TParryPostState();
		}
		return {};
	}

	TState handleInput(Context& context, float delta) override {
		if (context.input->isActionPressed(EInputAction::JUMP)) {
			return TInAirState();
		}
		return {};
	}

	TState deferredPhysicsProcess(Context& context, float delta) {
		if (!context.physics.collided) {
			return {};
		}

		utils::revertRigidbodyCollisionSlide(context);

		return {};
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
