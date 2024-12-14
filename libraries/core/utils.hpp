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

// template <typename T, typename Variant>
// struct isTypeInVariant;
// template <typename T, typename... Types>
// struct isTypeInVariant<T, std::variant<Types...>> : std::disjunction<std::is_same_v<T, Types>...> {};
// template <typename T, typename Variant>
// inline constexpr bool isTypeInVariantV = isTypeInVariant<T, Variant>::value;
