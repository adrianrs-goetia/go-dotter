#pragma once

#include <map>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

inline std::string toString(const std::vector<std::string>& vec) {
	std::stringstream ss;
	for (const auto& v : vec) {
		ss << v << " ";
	}
	return ss.str();
}

// helper type for the visitor #4
template <class... Ts>
struct overloaded : Ts... {
	using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// Variant cast
template <class... Args>
struct variantCastProxyLref {
	std::variant<Args...>& v;

	template <class... ToArgs>
	constexpr operator std::variant<ToArgs...>() && {
		return std::visit([](auto&& arg) -> std::variant<ToArgs...> { return std::forward<decltype(arg)>(arg); }, v);
	}
};

template <class... Args>
struct variantCastProxyConstLref {
	const std::variant<Args...>& v;

	template <class... ToArgs>
	constexpr operator std::variant<ToArgs...>() && {
		return std::visit([](auto&& arg) -> std::variant<ToArgs...> { return std::forward<decltype(arg)>(arg); }, v);
	}
};

template <class... Args>
struct variantCastProxyRref {
	std::variant<Args...>&& v;

	template <class... ToArgs>
	constexpr operator std::variant<ToArgs...>() && {
		return std::visit([](auto&& arg) -> std::variant<ToArgs...> { return std::forward<decltype(arg)>(arg); }, v);
	}
};

template <class... Args>
constexpr variantCastProxyLref<Args...> variantCast(std::variant<Args...>& v) {
	return { v };
}

template <class... Args>
constexpr variantCastProxyConstLref<Args...> variantCast(const std::variant<Args...>& v) {
	return { v };
}

template <class... Args>
constexpr variantCastProxyRref<Args...> variantCast(std::variant<Args...>&& v) {
	return { v };
}

/**
 * Common static constexpr functions which should return literals
 * Mostly for debug info when dealing with variants and quickly reading what type it is
 *
 * To be usable within cases where we call;
 * (auto a){ a.Foo(); }
 * Without having to deal with std::decay_t<decltype(a)>::Name or w/e
 */
#define TYPE(type)                                                                                                     \
public:                                                                                                                \
	static constexpr auto Name() {                                                                                     \
		return #type;                                                                                                  \
	}

// GDCLASS + TYPE for debug info
#define GDTYPE(type, inheritance)                                                                                      \
	TYPE(inheritance::type)                                                                                            \
	GDCLASS(type, inheritance)
