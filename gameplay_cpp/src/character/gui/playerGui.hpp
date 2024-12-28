#pragma once

#include <core/core.hpp>

#include "seameter.hpp"

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/panel.hpp>
#include <godot_cpp/classes/progress_bar.hpp>

#include <configparams.hpp>

class PlayerGui : public godot::Control {
	GDTYPE(PlayerGui, godot::Control)

	Seameter* _seameter = nullptr;

public:
	static void _bind_methods() {
	}

	void _enter_tree() override {
		_seameter = get_node<Seameter>("Seameter");

		RETURN_IF_EDITOR(void())
		ASSERTNN(_seameter)
	}

	template <typename T>
	T& get() const {
		if constexpr (std::is_same_v<T, Seameter>){
			return *_seameter;
		}
	}
};
