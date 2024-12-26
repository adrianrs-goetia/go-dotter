#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <managers/inputManager.h>
#include <configparams.hpp>

#include <components/animation.hpp>
#include <components/attackInstigator.hpp>

#include <debugdraw3d/api.h>

namespace fsm::player {

class AttackState : public BaseState {
	TYPE(AttackState)

private:
	Timestamp m_enterTimestamp;
	Timestamp m_exitTimestamp;

	ConfigParam::Player::Attack param;

public:
	TState getType() const override {
		return TAttackState();
	}

	bool canEnter() const override {
		const bool canEnter = !m_exitTimestamp.timestampWithinTimeframe(param.cooldown());
		if (!canEnter) {
			LOG(DEBUG, "Player AttackState still on cooldown")
		}
		return canEnter;
	}

	TState enter(Context& context) override {
		context.anim->doAttack();
		m_enterTimestamp.setTimestamp();
		context.attack->setComponentEnabled(true);
		context.attack->setAttackStrength(param.strength());
		return {};
	}

	TState exit(Context& context) override {
		context.anim->dontAttack();
		m_exitTimestamp.setTimestamp();
		context.attack->setComponentEnabled(false);
		context.anim->playAnimation(ComponentAnimation::EAnim::NONE);
		return {};
	}

	TState integrateForces(Context& context, godot::PhysicsDirectBodyState3D* state) override {
		const auto delta = state->get_step();
		state->set_linear_velocity(Vector3());

		DebugDraw::Position(
			godot::Transform3D(Basis(), godot::Vector3(context.physics.position + godot::Vector3(0, 1, 0))),
			godot::Color(0.7, 0, 0.5),
			delta);

		if (!m_enterTimestamp.timestampWithinTimeframe(param.timeout())) {
			return TOnGroundState();
		}

		return {};
	}

	TState handleInput(Context& context, float delta) override {
		return {};
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
