#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <configHandler.h>
#include <debugdraw3d/api.h>
#include <managers/inputManager.h>

#include <components/animation.hpp>
#include <components/parryInstigator.hpp>

#define CONFIG_PREFIX "player"

namespace fsm::player {

class ParryPostState : public BaseState {
	Timestamp m_enterTime;

public:
	TState getType() const override {
		return TParryPostState();
	}

	bool canEnter() const override {
		return true;
	}

	TState enter(Context& context) override {
		m_enterTime.setTimestamp();
		return {};
	}

	TState exit(Context& context) override {
		return {};
	}

	TState process(Context& context, float delta) override {
		return {};
	}

	TState physicsProcess(Context& context, float delta) {
		context.physics.velocity = Vector3();

		if (_passiveExit(context)) {
			return TOnGroundState();
		}

		const godot::Vector3 dir = context.parry->getLastParryTargetDir2D();
		context.anim->setRootTowardsVector(dir);

		return {};
	}

	TState handleInput(Context& context, float delta) override {
		if (context.input->isActionPressed(EInputAction::JUMP)) {
			auto* target = context.parry->getLastParryContactAssert();
			target->onAction(EventParryFreeze{ GETPARAM_F("parry", "freezetime") });
			return TParryJumpState();
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

private:
	bool _passiveExit(Context& context) {
		if (!context.parry->getLastParryContact()) {
			return true;
		}
		if (!m_enterTime.timestampWithinTimeframe(GETPARAM_F("parry", "postStateTime"))) {
			return true;
		}

		return false;
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
