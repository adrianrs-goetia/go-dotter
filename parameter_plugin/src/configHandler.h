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

	static std::string m_file;
	static ConfigReader m_reader;
	static parameter::Registry m_paramRegistry;

public:
	API static void addEntry(const parameter::StringKey& key, const parameter::Variant::EType type, const parameter::Callback&& callback);
	API static void removeEntry(const parameter::StringKey& key);

public:
	static void _bind_methods();

	void _enter_tree() override;
	void _exit_tree() override;
	void _ready() override;
	void _physics_process(double delta) override;

private:
	void _configUpdate();
	bool _readerCheckConfig();
	void _setReadStatusTrue();
};
