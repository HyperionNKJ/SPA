#pragma once

#include <unordered_set>
#include "Statement.h"

class QueryTokenizer {
public:
	QueryTokenizer(const std::string& newQuery);
	bool tokenize();
	std::vector<Statement> getStatements();
private:
	static const std::regex DECLARE_REGEX_STMT;
	static std::string const WHITESPACES;

	std::string query;
	std::vector<Statement> statements;

	void format();
	void split();
	bool tag();

	static bool isNoLeadingSpaceChar(char& target);
	static bool isNoPrecedingSpaceChar(char& target);
	static bool isDeclareStmt(const std::string& statement);
	static bool isSelectStmt(const std::string& statement);
};
