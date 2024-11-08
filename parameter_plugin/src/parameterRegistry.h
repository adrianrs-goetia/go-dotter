#pragma once

#include <core/core.hpp>

#include <functional>
#include <unordered_map>
#include <variant>

namespace parameter {

struct Variant {
	using Type = std::variant<bool, double, int, std::string>;

	Variant() = default;
	Variant(Type v)
		: value(v) {}

	//
	Type value;

	//
	void set(const Variant&& v) {
		ASSERT(value.index() == v.value.index(), "")
		value = v.value;
	}

	template <typename T>
	T& get() const {
		return std::get<T>(value);
	}
};

using StringKey = std::vector<std::string>;
using HashKey = size_t;
using Callback = std::function<void(const Variant&)>;

struct Registry {
	using Entry = std::tuple<Variant, Callback>;

private:
	std::unordered_map<HashKey, Entry> m_entries;

public:
	bool addEntry(const StringKey& key, const Callback&& callback) {
		auto hash = _getHashOfKey(key);
		auto [it, inserted] = m_entries.emplace(hash, Entry{ Variant{}, std::move(callback) });
		if (!inserted) {
			LOG(ERROR, "Failed insertion attempt at existing value:", toString(key).c_str())
			return false;
		}
		return true;
	}

	bool updateEntry(const StringKey& key, const Variant&& newValue) {
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

private:
	HashKey _getHashOfKey(const StringKey& val) {
		std::stringstream ss;
		for (const auto& v : val) {
			ss << v;
		}
		return std::hash<std::string>{}(ss.str());
	}
};

} //namespace parameter
