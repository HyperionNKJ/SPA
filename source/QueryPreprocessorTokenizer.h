#pragma once

#include <regex>
#include <unordered_set>

class QueryPreprocessorTokenizer {
public:
	QueryPreprocessorTokenizer(const std::string& newQuery);
	bool tokenize();
	std::vector<std::string> getStatements();
private:
	static const std::regex DECLARE_REGEX_STMT;
	static std::string const WHITESPACES;

	std::string query;
	std::vector<std::string> statements;

	void split();
	bool validateStatement();

	static bool isDeclareStatement(const std::string& statement);
	static bool isSelectStatement(const std::string& statement);
};
