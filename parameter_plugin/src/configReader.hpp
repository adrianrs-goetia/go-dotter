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
	struct Cache {
		std::filesystem::file_time_type lastWriteTime;
		json jcache;
	};
	std::map<std::string, Cache> m_cache;

public:
	ConfigReader() {
		m_cache.clear();
	}

	bool hasEmptyCache() const {
		return m_cache.empty();
	}

	bool checkFileChanged(const std::string& filepath) {
		if (std::filesystem::exists(filepath)) {
			const auto writeTime = std::filesystem::last_write_time(filepath);
			auto& cache = m_cache[filepath];
			if (writeTime != cache.lastWriteTime) {
				LOG(DEBUG, "Config filepath changed")
				cache.lastWriteTime = writeTime;
				return true;
			}
		}
		else {
			LOG(ERROR, "Config filepath was not found!")
		}
		return false;
	}

	/**
	 * If parse comes at awkward timing, the parsing would always fail so
	 * no reason to read the values at that exact moment
	 */
	bool parseFile(const std::string& filepath) {
		std::ifstream ifstream(filepath, std::ios_base::in);
		if (ifstream.is_open()) {
			try {
				std::stringstream ss;
				ss << ifstream.rdbuf();
				m_cache[filepath].jcache = json::parse(ss.str(), nullptr, true, true);
				return true;
			}
			catch (const json::parse_error& e) {
				LOG(WARN, "ConfigHandler could not parse config.cache: ", e.what())
			}
		}
		else {
			LOG(ERROR, "SHOULD NEVER HAPPEN. ConfigReader could not open file: ", filepath.c_str())
		}
		return false;
	}

	/**
	 */
	parameter::Variant getValue(const parameter::StringKey& arguments, const std::string& filepath) {
		json param;
		auto& cachedJson = m_cache[filepath].jcache;
		try {
			switch (arguments.size()) {
				case 1:
					param = cachedJson.at(arguments.at(0));
					break;
				case 2:
					param = cachedJson.at(arguments.at(0)).at(arguments.at(1));
					break;
				case 3:
					param = cachedJson.at(arguments.at(0)).at(arguments.at(1)).at(arguments.at(2));
					break;
				case 4:
					param = cachedJson.at(arguments.at(0)).at(arguments.at(1)).at(arguments.at(2)).at(arguments.at(3));
					break;
				case 5:
					param = cachedJson.at(arguments.at(0))
								.at(arguments.at(1))
								.at(arguments.at(2))
								.at(arguments.at(3))
								.at(arguments.at(4));
					break;
				case 6:
					param = cachedJson.at(arguments.at(0))
								.at(arguments.at(1))
								.at(arguments.at(2))
								.at(arguments.at(3))
								.at(arguments.at(4))
								.at(arguments.at(5));
					break;
				case 7:
					param = cachedJson.at(arguments.at(0))
								.at(arguments.at(1))
								.at(arguments.at(2))
								.at(arguments.at(3))
								.at(arguments.at(4))
								.at(arguments.at(5))
								.at(arguments.at(6));
					break;
				default: {
					LOG(ERROR, "Unexpected amount of arguments when getting config.cache value:", arguments.size())
					return {};
				}
			}
			if (param.is_boolean()) {
				return parameter::Variant(param.get<bool>());
			}
			else if (param.is_number_integer()) {
				return parameter::Variant(param.get<int>());
			}
			else if (param.is_number_float()) {
				return parameter::Variant(param.get<double>());
			}
			else if (param.is_string()) {
				return parameter::Variant(param.get<std::string>());
			}
			else {
				LOG(ERROR, "Unexpected type in json at:", toString(arguments).c_str())
			}
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

	void updateParameterRegistry(parameter::Registry& registry, const std::string& filepath) {
		// Parse json for all keys an objects
		parameter::StringKey key;
		_parseJsonObject(registry, key, m_cache[filepath].jcache, filepath);
	}

private:
	void _parseJsonObject(
		parameter::Registry& registry, parameter::StringKey& key, const json& object, const std::string filepath) {
		for (const auto& entry : object.items()) {
			key.push_back(entry.key());

			const auto& val = entry.value();
			if (val.is_boolean() || val.is_number_float() || val.is_number_integer() || val.is_string()) {
				auto val = getValue(key, filepath);
				registry.setEntry(key, std::move(val));
				key.pop_back();
			}
			else if (val.is_object()) {
				_parseJsonObject(registry, key, entry.value(), filepath);
			}
		}
		if (!key.empty()) {
			key.pop_back();
		}
	}
};
