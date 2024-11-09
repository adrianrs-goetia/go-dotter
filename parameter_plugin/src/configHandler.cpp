#include "configHandler.h"

#include <godot_cpp/classes/timer.hpp>

constexpr float g_readingInterval = 1.5f;

using namespace godot;

parameter::Registry ConfigHandler::m_paramRegistry;

parameter::Registry& ConfigHandler::getRegistry() { return m_paramRegistry; }

void ConfigHandler::_bind_methods() {}

void ConfigHandler::_enter_tree() {
	RETURN_IF_EDITOR(void())

	m_readerTimer = memnew(Timer);

	ASSERT_NOTNULL(m_readerTimer)

	add_child(m_readerTimer);
	m_readerTimer->connect("timeout", callable_mp(this, &ConfigHandler::_setReadStatusTrue));
	_setReadStatusTrue();
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
		if (auto parsedFile = m_reader.parseFile(m_file)) { _updateParameterRegistry(parsedFile.value()); }
	}
}

bool ConfigHandler::_readerCheckConfig() {
	if (m_readNextFrame) {
		m_readNextFrame = false;
		if (m_reader.checkFileChanged(m_file)) { return true; }
	}
	return false;
}

void ConfigHandler::_setReadStatusTrue() {
	m_readNextFrame = true;
	m_readerTimer->start(g_readingInterval);
}

void ConfigHandler::_updateParameterRegistry(const json& parsedJson) {
	// Parse json for all keys an objects
	parameter::StringKey key;
	for (const auto& entry : parsedJson.items()) {
		key.push_back(entry.key());

		// updateEntry in registry at the given key
		const auto& val = entry.value();
		if (val.is_boolean() || val.is_number_float() || val.is_number_integer() || val.is_string()) {
			auto val = m_reader.getValue(parsedJson, key);
			m_paramRegistry.updateEntry(key, val);
			key.clear();
		}
	}
}
