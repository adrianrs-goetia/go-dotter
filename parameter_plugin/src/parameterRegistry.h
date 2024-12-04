#pragma once

#include <core/core.hpp>

#include <functional>
#include <unordered_map>
#include <variant>

class ConfigReader;

namespace parameter {

struct API Variant {
	using Type = std::variant<bool, double, int, std::string>;

	Variant() = default;
	Variant(Type v)
		: value(v) {}

	Type value;

	void set(const Variant&& v) {
		value = v.value;
	}

	template <typename T>
	T get() const {
		return std::get<T>(value);
	}
};

using StringKey = std::vector<std::string>;
using HashKey = size_t;

class API Registry {
	std::unordered_map<HashKey, Variant> m_entries;

public:
	bool setEntry(const StringKey& key, const Variant&& newValue);

	Variant getEntry(const StringKey& key);

private:
	HashKey _getHashOfKey(const StringKey& val);
};

} //namespace parameter
