#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <configHandler.h>
#include <managers/inputManager.h>

#include <components/grappleInstigatorComponent.h>
#include <components/grappleTargetComponent.h>

#define CONFIG_PREFIX "player"

namespace fsm::player {

class GrappleLaunchState : public BaseState {
public:
	virtual bool canEnter() const override {
		return true;
	}

	TState getType() const override {
		return TGrappleLaunchState();
	}

	TState enter(Context& context) override {
		// TODO... What to do here other than launch?
		GrappleTargetComponent::LaunchContext launch = context.grapple->launch(GETPARAM_D("grapple", "launchStrength"));
		if (launch.type != GrappleTargetComponent::LaunchType::INSTIGATOR_ANCHOR &&
				launch.type != GrappleTargetComponent::LaunchType::BOTH_ANCHOR) {
			context.physics.velocity = launch.impulse;
		}
		return TInAirState();
	}

	TState exit(Context& context) override {
		return {};
	}

	TState process(Context& context, float delta) override {
		return {};
	}

	TState physicsProcess(Context& context, float delta) override {
		return {};
	}

	TState handleInput(Context& context, float delta) override {
		return {};
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
