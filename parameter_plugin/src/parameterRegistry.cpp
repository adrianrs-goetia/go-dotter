#include "parameterRegistry.h"

#include "configReader.h"

using namespace parameter;

bool parameter::Registry::setEntry(const StringKey& key, const Variant&& newValue) {
	auto hash = _getHashOfKey(key);
	m_entries[hash].set(std::move(newValue));
	return true;
}

Variant parameter::Registry::getEntry(const StringKey& key) {
	auto it = m_entries.find(_getHashOfKey(key));
	ASSERT_MSG(it != m_entries.end(), toString(key).c_str())
	return it->second;
}

HashKey parameter::Registry::_getHashOfKey(const StringKey& val) {
	std::stringstream ss;
	for (const auto& v : val) {
		ss << v;
	}
	return std::hash<std::string>{}(ss.str());
}
