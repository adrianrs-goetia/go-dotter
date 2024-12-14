#pragma once

#include "../typedef.h"

namespace fsm::projectile {

class Parried : public BaseState {
public:
	TState getType() const {
		return TParried();
	}

	TState enter(Context& context);

	TState exit(Context& context);

	TState physicsProcess(Context& context, float delta) override;

	TState handleExternalAction(Context& context, const ExternalAction& action) override;
};

} //namespace fsm::projectile