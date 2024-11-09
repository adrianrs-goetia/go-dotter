#include "configReader.h"

bool ConfigReader::hasCachedFile() const { return !m_cachedJson.empty(); }

bool ConfigReader::checkFileChanged(const std::string& filePath) {
	if (std::filesystem::exists(filePath)) {
		const auto writeTime = std::filesystem::last_write_time(filePath);
		if (writeTime != m_lastWriteTime) {
			LOG(INFO, "Config filePath changed")
			m_lastWriteTime = writeTime;
			return true;
		}
	}
	else { LOG(ERROR, "Config filePath was not found!") }
	return false;
}

bool ConfigReader::parseFile(const std::string& filePath) {
	std::ifstream ifstream(filePath, std::ios_base::in);
	if (ifstream.is_open()) {
		try {
			std::stringstream ss;
			ss << ifstream.rdbuf();
			m_cachedJson = json::parse(ss.str(), nullptr, true, true);
			return true;
		}
		catch (const json::parse_error& e) {
			LOG(WARN, "ConfigHandler could not parse config.json: ", e.what())
		}
	}
	else { LOG(ERROR, "SHOULD NEVER HAPPEN. ConfigReader could not open file: ", filePath.c_str()) }
	return false;
}

parameter::Variant ConfigReader::getValue(const parameter::StringKey& arguments) {
	json param;
	try {
		switch (arguments.size()) {
			case 1: param = m_cachedJson.at(arguments.at(0)); break;
			case 2: param = m_cachedJson.at(arguments.at(0)).at(arguments.at(1)); break;
			case 3: param = m_cachedJson.at(arguments.at(0)).at(arguments.at(1)).at(arguments.at(2)); break;
			case 4:
				param = m_cachedJson.at(arguments.at(0)).at(arguments.at(1)).at(arguments.at(2)).at(arguments.at(4));
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

void ConfigReader::updateParameterRegistry(parameter::Registry& registry) {
	// Parse json for all keys an objects
	parameter::StringKey key;
	_parseJsonObject(registry, key, m_cachedJson);
}

void ConfigReader::_parseJsonObject(parameter::Registry& registry, parameter::StringKey& key, const json& object) {
	for (const auto& entry : object.items()) {
		key.push_back(entry.key());

		const auto& val = entry.value();
		if (val.is_boolean() || val.is_number_float() || val.is_number_integer() || val.is_string()) {
			auto val = getValue(key);
			registry.updateEntry(key, val);
			key.pop_back();
		}
		else if (val.is_object()) { _parseJsonObject(registry, key, entry.value()); }
	}
	if (!key.empty()) { key.pop_back(); }
}
