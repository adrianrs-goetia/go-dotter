#include "parameterRegistry.h"

#include "configReader.h"

using namespace parameter;

bool parameter::Registry::addEntry(const StringKey& key, const Variant::EType type, const Callback&& callback) {
	// Create the entry
	auto hash = _getHashOfKey(key);
	auto [it, inserted] = m_entries.emplace(hash, Entry{ Variant(type), std::move(callback) });
	if (!inserted) {
		LOG(ERROR, "Failed insertion attempt at existing value:", toString(key).c_str())
		return false;
	}

	return true;
}

bool parameter::Registry::removeEntry(const StringKey& key) {
	auto eraseNum = m_entries.erase(_getHashOfKey(key));
	if (!eraseNum) {
		LOG(ERROR, "Tried to remove non existing field:", toString(key).c_str())
		return false;
	}
	return true;
}

bool parameter::Registry::updateEntry(const StringKey& key, const Variant& newValue) {
	if (m_entries.empty()) { return false; }

	auto it = m_entries.find(_getHashOfKey(key));
	if (it == m_entries.end()) {
		LOG(ERROR, "Failed to update registry at non-existing entry:", toString(key).c_str())
		return false;
	}
	auto& [value, callback] = (it->second);
	value.set(std::move(newValue));
	callback(value);
	return true;
}

HashKey parameter::Registry::_getHashOfKey(const StringKey& val) {
	std::stringstream ss;
	for (const auto& v : val) {
		ss << v;
	}
	return std::hash<std::string>{}(ss.str());
}
