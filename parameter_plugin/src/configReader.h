#pragma once

#include <core/core.hpp>

#include <nlohmann/json.hpp>

#include <filesystem>
#include <optional>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

class ConfigReader {
	std::unordered_map<std::string, std::filesystem::file_time_type> m_lastWriteTime;

private:
public:
	bool checkFileChanged(const std::string& filePath);
	/**
	 * If parse comes at awkward timing, the parsing would always fail so
	 * no reason to read the values at that exact moment
	 */
	std::optional<json> parseFile(const std::string& filePath);

	/**
	 * \param args have to be of a standard character type, string char[] etc...
	 * Will currently use nlohmann::json assert if element does not exist and cause a crash
	 */
	template <typename T, typename... Args>
	T getValue(const json& jsonParams, const Args&... args) {
		const std::vector<std::string> arguments = { args... };
		const size_t size = arguments.size();
		try {
			switch (arguments.size()) {
				case 1: return jsonParams[arguments.at(0)];
				case 2: return jsonParams[arguments.at(0)][arguments.at(1)];
				case 3: return jsonParams[arguments.at(0)][arguments.at(1)][arguments.at(2)];
				case 4: return jsonParams[arguments.at(0)][arguments.at(1)][arguments.at(2)][arguments.at(3)];
				default: break;
			}
		}
		catch (const json::parse_error& e) {
			LOG(ERROR, "Could not access value: ", e.what())
		}
		return {};
	}
};
