#pragma once

#include "../typedef.h"

namespace godot{
	class Timer;
}

namespace fsm::projectile {

class Launched : public BaseState {
	godot::Timer* m_timer = nullptr;

public:
	TState getType() const {
		return TLaunched();
	}

	TState enter(Context& context) override;

	TState exit(Context& context) override;

	TState handleExternalAction(Context& context, const ExternalAction& action) override;
};

} //namespace fsm::projectile