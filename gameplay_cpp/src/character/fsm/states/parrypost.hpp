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
public:
	TState getType() const override {
		return TParryPostState();
	}

	bool canEnter() const override {
		return true;
	}

	TState enter(Context& context) override {
		return {};
	}

	TState exit(Context& context) override {
		return {};
	}

	TState process(Context& context, float delta) override {
		return {};
	}

	TState physicsProcess(Context& context, float delta) {
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
};

} //namespace fsm::player

#undef CONFIG_PREFIX
