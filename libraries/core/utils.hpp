#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <map>

inline std::string toString(const std::vector<std::string>& vec) {
	std::stringstream ss;
	for (const auto& v : vec) {
		ss << v << " ";
	}
	return ss.str();
}