#pragma once

#include <core/core.hpp>

#include <godot_cpp/classes/button.hpp>

#include <functional>

class GuiButton : public godot::Button {
	GDTYPE(GuiButton, godot::Button)

	std::function<void()> _onpressed;

public:
	static void _bind_methods() {}

	void _enter_tree() {
		// connect("pressed", callable_mp(b, &ButtonBinder::emit));
	}

	void _pressed() override {
		if (_onpressed) {
			_onpressed();
		}
	}

	void setOnPressed(std::function<void()>&& f) {
		_onpressed = f;
	}
};
