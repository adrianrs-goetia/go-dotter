#include "configReader.h"

#include <godot_cpp/classes/timer.hpp>

#include <filesystem>

constexpr float g_readingInterval = 1.5f;

using namespace godot;

void ConfigReader::_bind_methods() {}

void ConfigReader::_enter_tree() {
	RETURN_IF_EDITOR(void())

	m_readerTimer = memnew(Timer);

	ASSERT_NOTNULL(m_readerTimer)

	add_child(m_readerTimer);
	m_readerTimer->connect("timeout", callable_mp(this, &ConfigReader::setReadStatusTrue));
	setReadStatusTrue();
}

void ConfigReader::_physics_process(double delta) {
	RETURN_IF_EDITOR(void())

	if (m_readNextFrame) {
		m_readNextFrame = false;
		if (auto newWriteTime = checkFileChanged()) {
			// Read changes
			LOG(WARN, "Config file changed")
			m_lastWriteTime = newWriteTime.value();
		}
	}
}

void ConfigReader::setReadStatusTrue() {
	m_readNextFrame = true;
	m_readerTimer->start(g_readingInterval);
}

std::optional<std::filesystem::file_time_type> ConfigReader::checkFileChanged() const {
	if (std::filesystem::exists(m_file)) {
		LOG(INFO, "Config file found")
		const auto writeTime = std::filesystem::last_write_time(m_file);
		if (writeTime != m_lastWriteTime) { return writeTime; }
		return {};
	}
	LOG(ERROR, "Config file was not found!")
	return {};
}
