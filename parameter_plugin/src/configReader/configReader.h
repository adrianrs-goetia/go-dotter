#pragma once

#include <core/core.hpp>

#include <godot_cpp/classes/node.hpp>

namespace godot{
	class Timer;
}

class ConfigReader : public godot::Node {
	GDCLASS(ConfigReader, godot::Node)

	godot::Timer* m_readerTimer = nullptr;

public:
	static void _bind_methods();

	void _enter_tree() override;
	void _physics_process(double delta) override;
};