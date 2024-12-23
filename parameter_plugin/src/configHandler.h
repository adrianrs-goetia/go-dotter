#pragma once

#include "parameterRegistry.h"
#include <core/core.hpp>
#include <memory>

#include <godot_cpp/classes/node.hpp>

class ConfigReader;

namespace godot {
class Timer;
}

class ConfigHandler : public godot::Node {
	GDCLASS(ConfigHandler, godot::Node)

public:
	struct Files {
		static constexpr auto Config = "config.json";
		static constexpr auto Application = "application.json";
	};

private:
	bool m_isReady = false;

	godot::Timer* m_readerTimer = nullptr;
	bool m_readNextFrame = false;

	static parameter::Registry m_paramRegistry;

	std::vector<std::string> m_files;

public:
	template <typename T>
	API static T getParam(const parameter::StringKey& key, const std::string& file) {
		return ConfigHandler::_getParamImpl(key, file).get<T>();
	}
	API static parameter::Variant _getParamImpl(const parameter::StringKey& key, const std::string& file);

public:
	ConfigHandler();

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

#define GETPARAMGLOBAL_D(...) ConfigHandler::getParam<double>({ __VA_ARGS__ }, ConfigHandler::Files::Config)
#define GETPARAMGLOBAL_F(...) (float)ConfigHandler::getParam<double>({ __VA_ARGS__ }, ConfigHandler::Files::Config)
#define GETPARAMGLOBAL_I(...) ConfigHandler::getParam<int>({ __VA_ARGS__ }, ConfigHandler::Files::Config)
#define GETPARAMGLOBAL_B(...) ConfigHandler::getParam<bool>({ __VA_ARGS__ }, ConfigHandler::Files::Config)
#define GETPARAMGLOBAL_S(...)                                                                                          \
	ConfigHandler::getParam<std::string>({ __VA_ARGS__ }, ConfigHandler::Files::Config).c_str()
/**
 * Requires #definition CONFIG_PREFIX
 *
 * e.g.
 * #define CONFIG_PREFIX "player", "parry"
 * to access that subobject in config.json by default
 */
#define GETPARAM_D(...) GETPARAMGLOBAL_D(CONFIG_PREFIX, __VA_ARGS__)
#define GETPARAM_F(...) GETPARAMGLOBAL_F(CONFIG_PREFIX, __VA_ARGS__)
#define GETPARAM_I(...) GETPARAMGLOBAL_I(CONFIG_PREFIX, __VA_ARGS__)
#define GETPARAM_B(...) GETPARAMGLOBAL_B(CONFIG_PREFIX, __VA_ARGS__)
#define GETPARAM_S(...) GETPARAMGLOBAL_S(CONFIG_PREFIX, __VA_ARGS__)

#define GETAPPPARAM_D(...) ConfigHandler::getParam<double>({ __VA_ARGS__ }, ConfigHandler::Files::Application)
#define GETAPPPARAM_F(...) (float)ConfigHandler::getParam<double>({ __VA_ARGS__ }, ConfigHandler::Files::Application)
#define GETAPPPARAM_I(...) ConfigHandler::getParam<int>({ __VA_ARGS__ }, ConfigHandler::Files::Application)
#define GETAPPPARAM_B(...) ConfigHandler::getParam<bool>({ __VA_ARGS__ }, ConfigHandler::Files::Application)
#define GETAPPPARAM_S(...)                                                                                             \
	ConfigHandler::getParam<std::string>({ __VA_ARGS__ }, ConfigHandler::Files::Application).c_str()

