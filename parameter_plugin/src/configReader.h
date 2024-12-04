#pragma once

#include <core/core.hpp>

#include "parameterRegistry.h"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>
#include <optional>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

class ConfigReader {
	std::filesystem::file_time_type m_lastWriteTime;
	json m_cachedJson;

public:
	ConfigReader() { m_cachedJson.clear(); }

	bool hasEmptyCache() const;

	bool checkFileChanged(const std::string& filePath);

	/**
	 * If parse comes at awkward timing, the parsing would always fail so
	 * no reason to read the values at that exact moment
	 */
	bool parseFile(const std::string& filePath);
	/**
	 */
	parameter::Variant getValue(const parameter::StringKey& arguments);

	void updateParameterRegistry(parameter::Registry& registry);

private:
	void _parseJsonObject(parameter::Registry& registry, parameter::StringKey& key, const json& object);
};
