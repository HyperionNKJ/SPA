#pragma once
#include <regex>
#include <string>

using namespace std;

class QueryPreprocessorFormatter {
public:
	QueryPreprocessorFormatter(std::string& newQuery);

	std::string getFormattedQuery();
private:
	std::string query;

	static const std::regex LEADING_SPACE;
	static const std::regex TRAILING_SPACE;

	void trim();
	void removeExtraWhitespace();
	void removeSpaces(string& original);
};