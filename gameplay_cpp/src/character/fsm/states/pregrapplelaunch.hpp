#pragma once

#include "../typedefs.hpp"
#include "_utils.hpp"
#include <configHandler.h>
#include <managers/inputManager.h>

#ifdef CONFIG_PREFIX
#undef CONFIG_PREFIX
#endif
#define CONFIG_PREFIX "player"

namespace fsm::player {

class PreGrappleLaunchState : public BaseState {
	TYPE(PreGrappleLaunchState)

public:
	TState getType() const override {
		return TPreGrappleLaunchState();
	}

	bool canEnter() const override {
		return true;
	}

	TState enter(Context& context) override {
		// TODO: PlayerState::can_enter_state() const ?? Make certain states not spammable?
		// TODO: camera adjustment and whatnot here?
		context.physics.velocity = godot::Vector3();
		return TGrappleLaunchState();
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

	TState deferredPhysicsProcess(Context& context, float delta) {
		return {};
	}
};

} //namespace fsm::player

#undef CONFIG_PREFIX
