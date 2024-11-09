#pragma once

#include <core/core.hpp>

#include "parameterRegistry.h"

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
	 */
	parameter::Variant getValue(const json& jsonParams, const parameter::StringKey& arguments) {
		json param;
		try {
			switch (arguments.size()) {
				case 1: param = jsonParams.at(arguments.at(0)); break;
				case 2: param = jsonParams.at(arguments.at(0)).at(arguments.at(1)); break;
				case 3: param = jsonParams.at(arguments.at(0)).at(arguments.at(1)).at(arguments.at(2)); break;
				case 4:
					param = jsonParams.at(arguments.at(0)).at(arguments.at(1)).at(arguments.at(2)).at(arguments.at(4));
					break;
				default: {
					LOG(ERROR, "Unexpected amount of arguments when getting config.json value:", arguments.size())
					return {};
				}
			}
			if (param.is_boolean()) { return parameter::Variant(param.get<bool>()); }
			else if (param.is_number_integer()) { return parameter::Variant(param.get<int>()); }
			else if (param.is_number_float()) { return parameter::Variant(param.get<double>()); }
			else if (param.is_string()) { return parameter::Variant(param.get<std::string>()); }
			else { LOG(ERROR, "Unexpected type in json at:", toString(arguments).c_str()) }
		}
		catch (...) {
			std::stringstream ss;
			{
				ss << "[ ";
				for (const auto& arg : arguments) {
					ss << "\'" << arg << "\'" << " ";
				}
				ss << "]";
			}
			LOG(ERROR, "Could not access value at:", ss.str().c_str())
		}
		return {};
	}
};
