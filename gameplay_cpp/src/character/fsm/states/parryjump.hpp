#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <configHandler.h>
#include <managers/inputManager.h>

#include <components/animation.hpp>
#include <components/parryInstigator.hpp>
#include <events/parry.hpp>

#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/gpu_particles3d.hpp>

#ifdef CONFIG_PREFIX
#undef CONFIG_PREFIX
#endif
#define CONFIG_PREFIX "player", "parry", "jump"

namespace fsm::player {

class ParryJumpState : public BaseState {
	TYPE(ParryJumpState)

private:
	Timestamp m_enterTime;
	Timestamp m_intangibilityTime;
	utils::CollisionLayerMask m_clm;

public:
	TState getType() const override {
		return TParryJumpState();
	}

	bool canEnter() const override {
		return true;
	}

	TState enter(Context& context) override {
		context.anim->doParry(true, false); //only animate upper
		m_enterTime.setTimestamp();

		m_clm = utils::disableCollision(context);

		auto* target = context.parry->getLastParryContactAssert();
		const auto targetPos = target->get_global_position() + godot::Vector3(0, 1, 0); // add margin to go above target
		const auto length = godot::Vector3(targetPos - context.physics.position);
		const auto dir = length.normalized();

		context.physics.horizontalForce = dir * length.length() * GETPARAM_F("impulse");
		return {};
	}

	TState exit(Context& context) override {
		utils::enableCollision(context, m_clm);
		return {};
	}

	TState process(Context& context, float delta) override {
		return {};
	}

	TState physicsProcess(Context& context, float delta) override {
		context.physics.horizontalForce.y +=
			(GETPARAMGLOBAL_D("gravityConstant") * GETPARAMGLOBAL_D("player", "gravityScale")) * delta;

		context.anim->rotateRootTowardsVector(
			context.physics.getVelocityDir2D(), delta, GETPARAMGLOBAL_D("player", "animation", "rootRotationSpeed"));

		if (!m_enterTime.timestampWithinTimeframe(GETPARAM_F("stateTime"))) {
			return TInAirState();
		}

		if (!m_intangibilityTime.timestampWithinTimeframe(GETPARAM_F("intagibilityTime"))) {
			utils::enableCollision(context, m_clm);
		}

		return {};
	}

	TState handleInput(Context& context, float delta) override {
		// @todo: should auto leave state after a certain point along the jump curve?

		if (context.input->isActionPressed(EInputAction::JUMP)) {
			// @todo: check if target is within range
			auto target = context.parry->getLastParryContactAssert();
			target->onAction({ EventParryJump() });

			auto newVel = context.input->getInputRelative3d(); // Expected to be horizontal
			newVel *= GETPARAM_D("doubleJumpHorizontalStrength");
			newVel.y = GETPARAM_D("doubleJumpImpulse");
			context.physics.horizontalForce = newVel;
			return TInAirState();
		}

		return {};
	}

	TState deferredPhysicsProcess(Context& context, float delta) {
		utils::moveSlideOwner(context);
		return {};
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
