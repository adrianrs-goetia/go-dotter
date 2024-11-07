#include "configReader.h"

#include <filesystem>
#include <fstream>

bool ConfigReader::checkFileChanged(const std::string& filePath) {
	if (std::filesystem::exists(filePath)) {
		const auto writeTime = std::filesystem::last_write_time(filePath);
		if (writeTime != m_lastWriteTime[filePath]) {
			LOG(INFO, "Config filePath changed")
			m_lastWriteTime[filePath] = writeTime;
			return true;
		}
	}
	else { LOG(ERROR, "Config filePath was not found!") }
	return false;
}

std::optional<json> ConfigReader::parseFile(const std::string& filePath) {
	std::ifstream ifstream(filePath, std::ios_base::in);
	if (ifstream.is_open()) {
		try {
			std::stringstream ss;
			ss << ifstream.rdbuf();
			return json::parse(ss.str(), nullptr, true, true);
		}
		catch (const json::parse_error& e) {
			LOG(WARN, "ConfigHandler could not parse config.json: ", e.what())
		}
	}
	else { LOG(ERROR, "SHOULD NEVER HAPPEN. ConfigReader could not open file: ", filePath.c_str()) }
	return std::nullopt;
}
