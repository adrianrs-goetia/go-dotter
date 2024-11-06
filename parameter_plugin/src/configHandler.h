#pragma once

#include "configReader.h"

#include <core/core.hpp>

#include <godot_cpp/classes/node.hpp>

namespace godot {
	class Timer;
}

class ConfigHandler : public godot::Node {
	GDCLASS(ConfigHandler, godot::Node)

	godot::Timer* m_readerTimer = nullptr;
	bool m_readNextFrame = false;

	ConfigReader m_reader;
	std::string m_file = "config.json";

public:
public:
	static void _bind_methods();

	void _enter_tree() override;
	void _physics_process(double delta) override;

private:
	bool _readerCheckConfig();
	void _setReadStatusTrue();
};
