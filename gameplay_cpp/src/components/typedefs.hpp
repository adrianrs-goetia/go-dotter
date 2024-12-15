#pragma once

#include <functional>
#include <memory>

class ParryTargetComponent;

struct ParryContact {
	using Get = std::function<ParryTargetComponent*()>;
	Get getTarget;

	ParryContact(Get get)
		: getTarget(std::move(get)) {}
};
