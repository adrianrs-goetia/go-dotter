#include "configHandler.h"

#include <godot_cpp/classes/timer.hpp>

constexpr float g_readingInterval = 1.5f;

using namespace godot;

parameter::Registry ConfigHandler::m_paramRegistry;
ConfigReader ConfigHandler::m_reader;
std::string ConfigHandler::m_file = "config.json";

void ConfigHandler::addEntry(
		const parameter::StringKey& key, const parameter::Variant::EType type, const parameter::Callback&& callback) {
	m_paramRegistry.addEntry(key, type, std::move(callback));
	if (!m_reader.hasCachedFile()) {
		if (m_reader.parseFile(m_file)) { m_reader.updateParameterRegistry(m_paramRegistry); }
	}
	m_paramRegistry.updateEntry(key, m_reader.getValue(key));
}

API void ConfigHandler::removeEntry(const parameter::StringKey& key) { m_paramRegistry.removeEntry(key); }

void ConfigHandler::_bind_methods() {}

void ConfigHandler::_enter_tree() {
	RETURN_IF_EDITOR(void())

	m_readerTimer = memnew(Timer);

	ASSERT_NOTNULL(m_readerTimer)

	add_child(m_readerTimer);
	m_readerTimer->connect("timeout", callable_mp(this, &ConfigHandler::_setReadStatusTrue));
	_setReadStatusTrue();
	/**
	 * @todo
	 * What to do when restarting a scene?
	 * Sending some kind of message or something?
	 */
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
		if (m_reader.parseFile(m_file)) { m_reader.updateParameterRegistry(m_paramRegistry); }
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
