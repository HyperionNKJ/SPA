#pragma once

#include <unordered_map>
#include "Type.h"

class QueryPreprocessorHelper {
public:
	static std::unordered_map<std::string, Type> const STRING_TO_TYPE;

	QueryPreprocessorHelper();
	static std::vector<std::string> split(const std::string& target, const char& delimiter);
	static Type getType(const std::string& typeString);
};
