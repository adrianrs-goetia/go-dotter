#include "configReader.h"

#include <filesystem>

bool ConfigReader::checkFileChanged(const std::string& file) {
	if (std::filesystem::exists(file)) {
		const auto writeTime = std::filesystem::last_write_time(file);
		if (writeTime != m_lastWriteTime) {
			LOG(INFO, "Config file changed")
			m_lastWriteTime = writeTime;
			return true;
		}
	}
	else { LOG(ERROR, "Config file was not found!") }
	return false;
}

bool ConfigReader::parseFile(const std::string& file) { return false; }
