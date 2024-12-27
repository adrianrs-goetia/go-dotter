#pragma once

#include <core/core.hpp>

#include <events/attack.hpp>
#include <events/parry.hpp>
#include <events/collision.hpp>

#include <optional>
#include <variant>

class Projectile;

namespace fsm::projectile {

using VExternalEvent = std::variant<EventAttack, EventParry, EventParryFreeze, EventParryJump, EventPlayerCollision>;

struct Context {
	Projectile* owner = nullptr;
	std::optional<VExternalEvent> forwardedAction;
};

struct TLaunched {};
struct TParried {};
struct TParryFreeze {};
struct TPostParryLaunched {};
using VState = std::variant<std::monostate, TLaunched, TParried, TParryFreeze, TPostParryLaunched>;

class BaseState {
public:
	virtual VState getType() const = 0;

	virtual bool canEnter() const {
		return true;
	};

	virtual VState enter(Context& context) = 0;

	virtual VState exit(Context& context) = 0;

	virtual VState handleExternalEvent(Context& context, VExternalEvent event) = 0;

	virtual VState process(Context& context, float delta) {
		return {};
	}

	virtual VState physicsProcess(Context& context, float delta) = 0;
};

} //namespace fsm::projectile