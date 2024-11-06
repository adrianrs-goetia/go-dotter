#pragma once

#include <core/core.hpp>

#include <filesystem>
#include <optional>

class ConfigReader {
	std::filesystem::file_time_type m_lastWriteTime;

public:
	bool checkFileChanged(const std::string& file);
	/**
	 * If parse comes at awkward timing, the parsing would always fail so
	 * no reason to read the values at that exact moment
	 */
	bool parseFile(const std::string& file);

	// ValueType <template Args... ??> getValue(const Args&&... args) getValue("player", "parry", "timing", "etc...")
	// getValue() arg1, arg2, arg3, arg...etc
};
