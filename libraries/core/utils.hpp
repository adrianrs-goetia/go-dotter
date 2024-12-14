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
