#pragma once

#include <core/core.hpp>

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/progress_bar.hpp>

#include <configparams.hpp>

class PlayerGui : public godot::Control {
	GDTYPE(PlayerGui, godot::Control)

	ConfigParam::Player::Seameter param;
	godot::ProgressBar* seameter = nullptr;

public:
	static void _bind_methods() {
		BIND_METHOD(PlayerGui, setupGui)
	}

	void _enter_tree() override {
		RETURN_IF_EDITOR(void())
		seameter = get_node<godot::ProgressBar>("Seameter/ProgressBar");
		ASSERTNN(seameter)
	}

	void setupGui() {
		ASSERTNN(seameter)
		seameter->set_step(1);
		seameter->set_max(param.max());
		seameter->set_min(0);
		seameter->set_value(param.base());
	}

	void update(int val) {
		ASSERTNN(seameter)
		seameter->set_value(std::clamp(val, 0, param.max()));
	}
};
