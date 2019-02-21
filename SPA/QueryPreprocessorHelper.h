#pragma once

class QueryPreprocessorHelper {
public:
	QueryPreprocessorHelper();
	static std::vector<std::string> split(const std::string& target, const char& delimiter);
};
