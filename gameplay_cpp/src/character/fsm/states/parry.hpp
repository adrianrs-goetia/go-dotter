#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <configHandler.h>
#include <managers/inputManager.h>

#include <components/parryInstigatorComponent.h>
#include <components/dataObjects/parryInstance.hpp>

#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>

#define CONFIG_PREFIX "player"

namespace fsm::player {

class ParryState : public BaseState {
private:
	Timestamp m_enterTimestamp;
	Timestamp m_exitTimestamp;

public:
	TState getType() const override {
		return TParryState();
	}

	bool canEnter() const override {
		const bool canEnter = !m_exitTimestamp.timestampWithinTimeframe(GETPARAM_D("parry", "cooldown"));
		if (!canEnter) {
			LOG(DEBUG, "Tried entering parryState before cooldown was ready")
		}
		return canEnter;
	}

	TState enter(Context& context) override {
		m_enterTimestamp.setTimestamp();
		context.physics.velocity = godot::Vector3(); // zero out velocity while in
		return {};
	}

	TState exit(Context& context) override {
		m_exitTimestamp.setTimestamp();
		return {};
	}

	TState process(Context& context, float delta) override {
		return {};
	}

	TState physicsProcess(Context& context, float delta) override {
		// Parry state timed out
		if (!m_enterTimestamp.timestampWithinTimeframe(GETPARAM_D("parry", "stateLength"))) {
			// Enter on ground by default, should discern if in air or onGround?
			return TOnGroundState();
		}
		if (const auto pi = context.parry->activateParry(
					ParryInstigatorComponent::ActivateParams{ context.input->getInputRelative3d(),
							GETPARAM_F("parry", "length"), GETPARAM_F("parry", "lift") })) {
			// Play effects
			context.audioVisual.audio->play();
			context.audioVisual.particles->set_global_position(pi->targetPosition);
			context.audioVisual.particles->set_global_basis(createBasisFromDirection(pi->getDirectionToTarget3D()));
			context.audioVisual.particles->restart();

			if (context.physics.isOnGround)
				return TOnGroundState();
			else
				return TInAirState();
		}
		return {};
	}

	TState handleInput(Context& context, float delta) override {
		if (context.input->isActionPressed(EInputAction::JUMP)) {
			return TInAirState();
		}
		return {};
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
