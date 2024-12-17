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

#define CONFIG_PREFIX "player", "parry"

namespace fsm::player {

class ParryJumpState : public BaseState {
private:
	Timestamp m_enterTime;

public:
	TState getType() const override {
		return TParryJumpState();
	}

	bool canEnter() const override {
		return true;
	}

	TState enter(Context& context) override {
		auto target = context.parry->getLastParryContactAssert();
		m_enterTime.setTimestamp();

		LOG(INFO, "parry jump")

		const auto targetPos = target->get_global_position() + godot::Vector3(0, 1, 0); // add margin to go above target
		const auto length = godot::Vector3(targetPos - context.physics.position);
		const auto dir = length.normalized();

		context.physics.velocity = dir * length.length() * GETPARAM_F("parryJumpStrength");
		return {};
	}

	TState exit(Context& context) override {
		return {};
	}

	TState process(Context& context, float delta) override {
		return {};
	}

	TState physicsProcess(Context& context, float delta) override {
		context.physics.velocity.y +=
			(GETPARAMGLOBAL_D("gravityConstant") * GETPARAMGLOBAL_D("player", "gravityScale")) * delta;

		context.anim->rotateRootTowardsVector(
			context.physics.getVelocityDir2D(), delta, GETPARAMGLOBAL_D("player", "animation", "rootRotationSpeed"));

		if (!m_enterTime.timestampWithinTimeframe(GETPARAM_F("parryJumpStateTime"))) {
			return TInAirState();
		}

		return {};
	}

	TState handleInput(Context& context, float delta) override {
		if (context.input->isActionPressed(EInputAction::JUMP)) {
			auto target = context.parry->getLastParryContactAssert();
			target->onAction({ EventParryJump() });

			context.physics.velocity.y = GETPARAM_D("parryDoubleJumpStrength");
			return TInAirState();
		}

		return {};
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
