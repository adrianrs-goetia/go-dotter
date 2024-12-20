#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <configHandler.h>
#include <managers/inputManager.h>

#include <components/animation.hpp>
#include <components/attackInstigator.hpp>

#include <debugdraw3d/api.h>

#define CONFIG_PREFIX "player"

namespace fsm::player {

class AttackState : public BaseState {
private:
	Timestamp m_enterTimestamp;
	Timestamp m_exitTimestamp;

public:
	TState getType() const override {
		return TAttackState();
	}

	bool canEnter() const override {
		const bool canEnter = !m_exitTimestamp.timestampWithinTimeframe(GETPARAM_D("attack", "cooldown"));
		if (!canEnter) {
			LOG(DEBUG, "Player AttackState still on cooldown")
		}
		return canEnter;
	}

	TState enter(Context& context) override {
		m_enterTimestamp.setTimestamp();
		context.attack->setComponentEnabled(true);
		context.attack->setAttackStrength(GETPARAM_F("attack", "strength"));
		context.anim->playAnimation(ComponentAnimation::EAnim::ATTACK);
		return {};
	}

	TState exit(Context& context) override {
		m_exitTimestamp.setTimestamp();
		context.attack->setComponentEnabled(false);
		context.anim->playAnimation(ComponentAnimation::EAnim::NONE);
		return {};
	}

	TState process(Context& context, float delta) override {
		return {};
	}

	TState physicsProcess(Context& context, float delta) override {
		DebugDraw::Position(
			godot::Transform3D(Basis(), godot::Vector3(context.physics.position + godot::Vector3(0, 1, 0))),
			godot::Color(0.7, 0, 0.5),
			delta);

		if (!m_enterTimestamp.timestampWithinTimeframe(GETPARAM_D("attack", "stateLength"))) {
			if (context.physics.isOnGround)
				return TOnGroundState();
			else
				return TInAirState();
		}

		context.physics.velocity *= 0;

		switch (context.attack->getAttackState()) {
			case ComponentAttackInstigator::EState::HIT: {
			}
			case ComponentAttackInstigator::EState::NOT_HIT: {
			}

			default:
				break;
		}
		return {};
	}

	TState handleInput(Context& context, float delta) override {
		return {};
	}

	TState deferredPhysicsProcess(Context& context, float delta) {
		return {};
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
