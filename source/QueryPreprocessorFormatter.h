#pragma once
#include <string>

class QueryPreprocessorFormatter {
public:
	QueryPreprocessorFormatter(std::string& query);
	std::string getFormattedQuery();
private:
	std::string query;
	void trim();
	void removeExtraWhitespace();
	void removeSpaces();
	void removeLeadingSpace();
	void removeTrailingSpace();
};