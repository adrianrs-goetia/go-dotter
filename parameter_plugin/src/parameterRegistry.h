#pragma once

#include <core/core.hpp>

#include <functional>
#include <unordered_map>
#include <variant>

class ConfigReader;

namespace parameter {

struct Variant {
	enum EType { BOOL, DOUBLE, INT, STRING };

	using Type = std::variant<bool, double, int, std::string>;

	Variant() = default;
	Variant(Type v)
		: value(v) {}
	Variant(EType type) {
		switch (type) {
			case BOOL: value = bool(); break;
			case DOUBLE: value = double(); break;
			case INT: value = int(); break;
			case STRING: value = std::string(); break;
		}
	}

	//
	Type value;

	//
	void set(const Variant&& v) {
		ASSERT(value.index() == v.value.index(), "")
		value = v.value;
	}

	template <typename T>
	T get() const {
		return std::get<T>(value);
	}
};

using StringKey = std::vector<std::string>;
using HashKey = size_t;
using Callback = std::function<void(const Variant&)>;

struct API Registry {
	using Entry = std::tuple<Variant, Callback>;

private:
	std::unordered_map<HashKey, Entry> m_entries;

public:
	bool addEntry(const StringKey& key, const Variant::EType type, const Callback&& callback);

	bool removeEntry(const StringKey& key);

	bool updateEntry(const StringKey& key, const Variant& newValue);

private:
	// void _setValue(const StringKey& key, std::unordered_map<HashKey, Entry>::iterator& it);
	HashKey _getHashOfKey(const StringKey& val);
};

} //namespace parameter
