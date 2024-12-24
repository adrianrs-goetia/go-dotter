#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <configHandler.h>
#include <debugdraw3d/api.h>
#include <managers/inputManager.h>

#include <components/animation.hpp>
#include <components/parryInstigator.hpp>

#ifdef CONFIG_PREFIX
#undef CONFIG_PREFIX
#endif
#define CONFIG_PREFIX "player", "parry", "post"

namespace fsm::player {

class ParryPostState : public BaseState {
	TYPE(ParryPostState)

private:
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

	TState integrateForces(Context& context, godot::PhysicsDirectBodyState3D* state) override {
		state->set_linear_velocity(godot::Vector3());

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
			target->onAction(EventParryFreeze{ GETPARAM_F("freezetime") });
			return TParryJumpState();
		}
		return {};
	}

private:
	bool _passiveExit(Context& context) {
		if (!context.parry->getLastParryContact()) {
			return true;
		}
		if (!m_enterTime.timestampWithinTimeframe(GETPARAM_F("stateTime"))) {
			return true;
		}

		return false;
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
