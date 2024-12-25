#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <debugdraw3d/api.h>
#include <managers/inputManager.h>
#include <configparams.hpp>

#include <components/animation.hpp>
#include <components/parryInstigator.hpp>

namespace fsm::player {

class ParryPostState : public BaseState {
	TYPE(ParryPostState)

	ConfigParam::Player::Parry::Post param;

private:
	Timestamp m_enterTime;
	bool m_isOnFloor{};

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

	TState integrateForces(Context& context, godot::PhysicsDirectBodyState3D* state) override {
		m_isOnFloor = utils::isOnFloor(*state);
		if (m_isOnFloor) {
			state->set_linear_velocity(godot::Vector3());
		}
		else {
			context.physics.movement.y -=
				ConfigParam::gravityConstant() * param.inair.gravityScaleOverride() * state->get_step();
			state->set_linear_velocity(context.physics.movement);
		}

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
			target->onAction({ EventParryJump() });
			return TParryJumpState();
		}
		return {};
	}

private:
	bool _passiveExit(Context& context) {
		if (!context.parry->getLastParryContact()) {
			return true;
		}
		if (!m_enterTime.timestampWithinTimeframe(param.timeout())) {
			return true;
		}

		return false;
	}
};

} //namespace fsm::player
