#include "parried.h"

#include "_utils.h"
#include <configHandler.h>
#include <npcs/projectile/projectile.h>

#define CONFIG_PREFIX "npcs", "projectile"

namespace fsm::projectile {

TState Parried::enter(Context& context) {
	return {};
}

TState Parried::exit(Context& context) {
	return {};
}

TState fsm::projectile::Parried::physicsProcess(Context& context, float delta) {
	if (context.owner->isOnGround()) {
		utils::death(context);
	}
	return {};
}

TState Parried::handleExternalAction(Context& context, const ExternalAction& action) {
	if (std::holds_alternative<AttackInstance>(action)) {
		auto& attackInstance = std::get<AttackInstance>(action);
		const auto dir = attackInstance.getDirection();
		context.owner->set_linear_velocity(dir * attackInstance.attackStrength);
		return TLaunched{};
	}
	return {};
}

} //namespace fsm::projectile
