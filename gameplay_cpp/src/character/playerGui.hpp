#pragma once

#include <core/core.hpp>

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/panel.hpp>
#include <godot_cpp/classes/progress_bar.hpp>

#include <configparams.hpp>

class PlayerGui : public godot::Control {
	GDTYPE(PlayerGui, godot::Control)

	ConfigParam::Player param;

	struct {
		godot::ProgressBar* progressbar = nullptr;
		int value;
	} seameter;

	GS_PACKEDSCENE_IMPL(_barslider, BarSlider)
	G_INT_IMPL(_maxnum, MaxNum, = 3)
	G_INT_IMPL(_num, Num, = 1)
	G_FLOAT_IMPL(_posY, PosY, = 0.f)
	G_VECTOR2_IMPL(_size, Size, = godot::Vector2())

public:
	static void _bind_methods() {
		METHOD_PROPERTY_IMPL(PlayerGui, MaxNum, INT)
		METHOD_PROPERTY_IMPL(PlayerGui, Num, INT)
		METHOD_PROPERTY_IMPL(PlayerGui, PosY, FLOAT)
		METHOD_PROPERTY_IMPL(PlayerGui, Size, VECTOR2)
		METHOD_PROPERTY_PACKEDSCENE_IMPL(PlayerGui, BarSlider)
		BIND_METHOD(PlayerGui, setupGui)
		BIND_METHOD(PlayerGui, setupProgressBarPanels)
	}

	void _enter_tree() override {
		seameter.progressbar = get_node<godot::ProgressBar>("Seameter/ProgressBar");
		if (seameter.progressbar) {
			call_deferred("setupGui");
			call_deferred("setupProgressBarPanels");
		}

		RETURN_IF_EDITOR(void())
		ASSERTNN(seameter.progressbar)
	}

	void setupGui() {
		ASSERTNN(seameter.progressbar)
		// seameter.value = param.seameter.base();
		seameter.value = getNum();
		seameter.progressbar->set_step(1);
		seameter.progressbar->set_min(0);
		// seameter.progressbar->set_max(param.seameter.max());
		seameter.progressbar->set_max(getMaxNum());
		seameter.progressbar->set_value(seameter.value);
	}

	void setupProgressBarPanels() {
		ASSERTNN(seameter.progressbar)
		auto children = seameter.progressbar->get_children();
		while (children.size()) {
			seameter.progressbar->remove_child(cast_to<Node>(children.pop_back()));
		}

		const auto pos = seameter.progressbar->get_position();
		const auto size = seameter.progressbar->get_size();
		const auto step = size.x / (float)std::max(1, _maxnum);
		const auto leftEdge = pos.x;

		for (int i = 1; i < std::max(1, _maxnum); i++) {
			auto slider = cast_to<godot::Panel>(_barslider->instantiate());
			ASSERTNN(slider)

			const auto xPos = (step * i);

			slider->set_anchors_preset(godot::Control::LayoutPreset::PRESET_BOTTOM_LEFT);
			slider->set_position(godot::Vector2(xPos, -(size.y + _posY)));
			slider->set_size(_size);

			seameter.progressbar->call_deferred("add_child", slider);
		}
	}

	void setNum(int val) {
		_num = val;
		RETURN_IF_NODE_NOT_READY(void())
		call_deferred("setupGui");
		call_deferred("setupProgressBarPanels");
	}
	void setMaxNum(int val) {
		_maxnum = val;
		RETURN_IF_NODE_NOT_READY(void())
		call_deferred("setupGui");
		call_deferred("setupProgressBarPanels");
	}
	void setPosY(int val) {
		_posY = val;
		RETURN_IF_NODE_NOT_READY(void())
		call_deferred("setupGui");
		call_deferred("setupProgressBarPanels");
	}
	void setSize(godot::Vector2 val) {
		_size = val;
		RETURN_IF_NODE_NOT_READY(void())
		call_deferred("setupGui");
		call_deferred("setupProgressBarPanels");
	}

	// returns true when seameter increments to max
	bool increment() {
		ASSERTNN(seameter.progressbar)
		seameter.value = std::min(++seameter.value, param.seameter.max());
		seameter.progressbar->set_value(seameter.value);
		return seameter.value == param.seameter.max();
	}

	// returns true when seameter decrements to zero
	bool decrement() {
		ASSERTNN(seameter.progressbar)
		seameter.value = std::max(--seameter.value, 0);
		seameter.progressbar->set_value(seameter.value);
		return seameter.value == 0;
	}
};
