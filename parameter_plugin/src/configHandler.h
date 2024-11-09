#pragma once

#include "configReader.h"
#include "parameterRegistry.h"

#include <core/core.hpp>

#include <godot_cpp/classes/node.hpp>

namespace godot {
class Timer;
}

class ConfigHandler : public godot::Node {
	GDCLASS(ConfigHandler, godot::Node)

private:
	bool m_isReady = false;

	godot::Timer* m_readerTimer = nullptr;
	bool m_readNextFrame = false;

	ConfigReader m_reader;
	std::string m_file = "config.json";

	static parameter::Registry m_paramRegistry;

public:
	API static parameter::Registry& getRegistry();

public:
	static void _bind_methods();

	void _enter_tree() override;
	void _ready() override;
	void _physics_process(double delta) override;

private:
	void _configUpdate();
	bool _readerCheckConfig();
	void _setReadStatusTrue();
	void _updateParameterRegistry(const json& parsedJson);
};
