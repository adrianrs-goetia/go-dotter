#pragma once

#include <core/core.hpp>

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/panel.hpp>
#include <godot_cpp/classes/progress_bar.hpp>

#include <configparams.hpp>

class Seameter : public godot::Control {
	GDTYPE(Seameter, godot::Control)

	ConfigParam::Player::Seameter param;

	godot::ProgressBar* _progressbar = nullptr;
	int _value;

	GS_PACKEDSCENE_IMPL(_barslider, BarSlider)
	G_FLOAT_IMPL(_posY, PosY, = 0.f)
	G_VECTOR2_IMPL(_size, Size, = godot::Vector2())

public:
	static void _bind_methods() {
		METHOD_PROPERTY_IMPL(Seameter, PosY, FLOAT)
		METHOD_PROPERTY_IMPL(Seameter, Size, VECTOR2)
		METHOD_PROPERTY_PACKEDSCENE_IMPL(Seameter, BarSlider)
		BIND_METHOD(Seameter, setupGui)
		BIND_METHOD(Seameter, setupProgressBarPanels)
	}

	void _enter_tree() override {
		_progressbar = getComponentOfNode<godot::ProgressBar>(this);
		if (_progressbar) {
			_progressbar->set_name("ProgressBar");
			call_deferred("setupGui");
			call_deferred("setupProgressBarPanels");
		}

		RETURN_IF_EDITOR(void())
		ASSERTNN(_progressbar)
	}

	void setupGui() {
		ASSERTNN(_progressbar)
		_value = param.base();
		_progressbar->set_step(1);
		_progressbar->set_min(0);
		_progressbar->set_max(param.max());
		_progressbar->set_value(_value);
	}

	void setupProgressBarPanels() {
		ASSERTNN(_progressbar)
		auto children = _progressbar->get_children();
		while (children.size()) {
			_progressbar->remove_child(cast_to<Node>(children.pop_back()));
		}

		const auto pos = _progressbar->get_position();
		const auto size = _progressbar->get_size();
		const auto step = size.x / (float)std::max(1, param.max());
		const auto leftEdge = pos.x;

		for (int i = 1; i < std::max(1, param.max()); i++) {
			auto slider = cast_to<godot::Panel>(_barslider->instantiate());
			ASSERTNN(slider)

			const auto xPos = (step * i);

			slider->set_anchors_preset(godot::Control::LayoutPreset::PRESET_BOTTOM_LEFT);
			slider->set_position(godot::Vector2(xPos, -(size.y + _posY)));
			slider->set_size(_size);

			_progressbar->call_deferred("add_child", slider);
		}
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
		ASSERTNN(_progressbar)
		_value = std::min(++_value, param.max());
		_progressbar->set_value(_value);
		return _value == param.max();
	}

	// returns true when seameter decrements to zero
	bool decrement() {
		ASSERTNN(_progressbar)
		_value = std::max(--_value, 0);
		_progressbar->set_value(_value);
		return _value == 0;
	}
};
