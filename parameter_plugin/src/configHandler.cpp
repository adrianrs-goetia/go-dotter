#include "configHandler.h"

#include <godot_cpp/classes/timer.hpp>

constexpr float g_readingInterval = 1.5f;

using namespace godot;

void ConfigHandler::_bind_methods() {}

void ConfigHandler::_enter_tree() {
	RETURN_IF_EDITOR(void())

	m_readerTimer = memnew(Timer);

	ASSERT_NOTNULL(m_readerTimer)

	add_child(m_readerTimer);
	m_readerTimer->connect("timeout", callable_mp(this, &ConfigHandler::_setReadStatusTrue));
	_setReadStatusTrue();
}

void ConfigHandler::_physics_process(double delta) {
	RETURN_IF_EDITOR(void())

	if (_readerCheckConfig()) {
		if (m_reader.parseFile(m_file)) {
			// Reader successfully parsed file
		}
		//
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
