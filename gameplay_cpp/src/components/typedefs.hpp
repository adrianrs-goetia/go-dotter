#pragma once

#include <functional>
#include <memory>

class ComponentParryTarget;

struct ParryContact {
	using Get = std::function<ComponentParryTarget*()>;
	Get getTarget;

	ParryContact(Get get)
		: getTarget(std::move(get)) {}
};
