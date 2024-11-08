#pragma once

#include <sstream>
#include <string>
#include <vector>

inline std::string toString(const std::vector<std::string>& vec) {
	std::stringstream ss;
	for (const auto& v : vec) {
		ss << v << " ";
	}
	return ss.str();
}