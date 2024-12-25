// #pragma once

// #include "../typedefs.hpp"
// #include "_utils.hpp"
// #include <configparams.hpp>
// #include <managers/inputManager.h>

// #include <components/grappleInstigator.hpp>
// #include <components/grappleTarget.hpp>

// #ifdef CONFIG_PREFIX
// #undef CONFIG_PREFIX
// #endif
// #define CONFIG_PREFIX "player"

// namespace fsm::player {

// class GrappleLaunchState : public BaseState {
// 	TYPE(GrappleLaunchState)

// public:
// 	virtual bool canEnter() const override {
// 		return true;
// 	}

// 	TState getType() const override {
// 		return TGrappleLaunchState();
// 	}

// 	TState enter(Context& context) override {
// 		// TODO... What to do here other than launch?
// 		ComponentGrappleTarget::LaunchContext launch = context.grapple->launch(GETPARAM_D("grapple", "launchStrength"));
// 		if (launch.type != ComponentGrappleTarget::LaunchType::INSTIGATOR_ANCHOR &&
// 			launch.type != ComponentGrappleTarget::LaunchType::BOTH_ANCHOR) {
// 			context.physics.horizontalForce = launch.impulse;
// 		}
// 		return TInAirState();
// 	}

// 	TState exit(Context& context) override {
// 		return {};
// 	}

// 	TState process(Context& context, float delta) override {
// 		return {};
// 	}

// 	TState physicsProcess(Context& context, float delta) override {
// 		return {};
// 	}

// 	TState handleInput(Context& context, float delta) override {
// 		return {};
// 	}

// 	TState deferredPhysicsProcess(Context& context, float delta) {
// 		return {};
// 	}
// };

// } //namespace fsm::player

// #undef CONFIG_PREFIX
