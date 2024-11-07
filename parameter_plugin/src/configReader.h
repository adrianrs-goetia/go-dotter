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
	auto getJsonField(const json& obj, const std::string& field) {
		if (obj.contains(field)) { return obj.at(field); }
	}

public:
	bool checkFileChanged(const std::string& filePath);
	/**
	 * If parse comes at awkward timing, the parsing would always fail so
	 * no reason to read the values at that exact moment
	 */
	std::optional<json> parseFile(const std::string& filePath);

	/**
	 * \param args have to be of a standard character type, string char[] etc...
	 * Does not assert that return type is correct 
	 */
	template <typename T>
	T getValue(const json& jsonParams, const std::vector<std::string>& arguments) {
		try {
			switch (arguments.size()) {
				case 1: return jsonParams.at(arguments.at(0));
				case 2: return jsonParams.at(arguments.at(0)).at(arguments.at(1));
				case 3: return jsonParams.at(arguments.at(0)).at(arguments.at(1)).at(arguments.at(2));
				case 4:
					return jsonParams.at(arguments.at(0)).at(arguments.at(1)).at(arguments.at(2)).at(arguments.at(4));
				default: break;
			}
		}
		catch (const std::exception& e) {
			std::stringstream ss;
			{
				ss << "[ ";
				for (const auto& arg : arguments){
					ss << "\'" << arg << "\'" << " ";
				}
				ss << "]";
			}
			// ss << "\n" << "Exception message: " << e.what();
			LOG(ERROR, "Could not access value at:", ss.str().c_str())
		}
		return {};
	}
};
