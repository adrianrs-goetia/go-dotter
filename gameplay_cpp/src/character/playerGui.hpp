#pragma once

#include <core/core.hpp>

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/progress_bar.hpp>

#include <configparams.hpp>

class PlayerGui : public godot::Control {
	GDTYPE(PlayerGui, godot::Control)

	ConfigParam::Player param;

	struct {
		godot::ProgressBar* progressbar = nullptr;
		int value;
	} seameter;

public:
	static void _bind_methods() {
		BIND_METHOD(PlayerGui, setupGui)
	}

	void _enter_tree() override {
		RETURN_IF_EDITOR(void())
		seameter.progressbar = get_node<godot::ProgressBar>("Seameter/ProgressBar");
		ASSERTNN(seameter.progressbar)
	}

	void setupGui() {
		ASSERTNN(seameter.progressbar)
		seameter.value = param.seameter.base();
		seameter.progressbar->set_step(1);
		seameter.progressbar->set_max(param.seameter.max());
		seameter.progressbar->set_min(0);
		seameter.progressbar->set_value(seameter.value);
	}

	bool increment() {
		ASSERTNN(seameter.progressbar)
		seameter.value = std::min(++seameter.value, param.seameter.max());
		seameter.progressbar->set_value(seameter.value);
		return seameter.value == param.seameter.max();
	}

    // returns true when seameter is 0
	bool decrement() {
		ASSERTNN(seameter.progressbar)
		seameter.value = std::max(--seameter.value, 0);
		seameter.progressbar->set_value(seameter.value);
		return seameter.value == 0;
	}
};
