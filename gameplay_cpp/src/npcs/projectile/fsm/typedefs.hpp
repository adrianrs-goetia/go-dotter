#pragma once

#include <core/core.hpp>

#include <components/dataObjects/attackInstance.h>
#include <components/dataObjects/parryInstance.hpp>

#include <variant>

class Projectile;

namespace fsm::projectile {

struct Context {
	Projectile* owner = nullptr;
};

using ExternalAction = std::variant<AttackInstance, ParryInstance, ParryUtilization>;

struct TLaunched {};
struct TParried {};
struct TPostParryLaunched {};
using TState = std::variant<std::monostate, TLaunched, TParried, TPostParryLaunched>;

class BaseState {
public:
	virtual TState getType() const = 0;

	virtual bool canEnter() const {
		return true;
	};

	virtual TState enter(Context& context) = 0;

	virtual TState exit(Context& context) = 0;

	virtual TState handleExternalAction(Context& context, const ExternalAction& action) = 0;

	virtual TState process(Context& context, float delta) {
		return {};
	}

	virtual TState physicsProcess(Context& context, float delta) = 0;
};

} //namespace fsm::projectile