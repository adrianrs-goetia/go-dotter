#include "configHandler.h"

#include "configReader.hpp"

#include <godot_cpp/classes/timer.hpp>

constexpr float g_readingInterval = 1.5f;
static ConfigReader g_reader;

using namespace godot;

parameter::Registry ConfigHandler::m_paramRegistry;

API parameter::Variant ConfigHandler::_getParamImpl(const parameter::StringKey& key, const std::string& file) {
	if (g_reader.hasEmptyCache()) {
		if (g_reader.parseFile(file)) {
			g_reader.updateParameterRegistry(m_paramRegistry, file);
		}
	}
	return m_paramRegistry.getEntry(key);
}

ConfigHandler::ConfigHandler() {
	m_files = {
		Files::Config,
		Files::Application,
	};
}

void ConfigHandler::_bind_methods() {}

void ConfigHandler::_enter_tree() {
	RETURN_IF_EDITOR(void())

	m_readerTimer = memnew(Timer);

	ASSERTNN(m_readerTimer)

	add_child(m_readerTimer);
	m_readerTimer->connect("timeout", callable_mp(this, &ConfigHandler::_setReadStatusTrue));
	_setReadStatusTrue();
	_configUpdate();
}

void ConfigHandler::_exit_tree() {
	RETURN_IF_EDITOR(void())
	//
}

void ConfigHandler::_ready() {
	m_isReady = true;
	_configUpdate();
}

void ConfigHandler::_physics_process(double delta) {
	RETURN_IF_EDITOR(void())

	_configUpdate();
}

void ConfigHandler::_configUpdate() {
	if (m_isReady && _readerCheckConfig()) {
		for (const auto& file : m_files) {
			if (g_reader.parseFile(file)) {
				g_reader.updateParameterRegistry(m_paramRegistry, file);
			}
		}
	}
}

bool ConfigHandler::_readerCheckConfig() {
	if (m_readNextFrame) {
		m_readNextFrame = false;
		for (const auto& file : m_files) {
			if (g_reader.checkFileChanged(file)) {
				return true;
			}
		}
	}
	return false;
}

void ConfigHandler::_setReadStatusTrue() {
	m_readNextFrame = true;
	m_readerTimer->start(g_readingInterval);
}
