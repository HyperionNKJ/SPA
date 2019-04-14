#pragma once
#include <regex>

class QueryPreprocessorTokenizer {
public:
	// Initializes a newly created QueryPreprocessorFormatterTokenizer.
	QueryPreprocessorTokenizer(const std::string& query);
	
	// Tokenises the query into statements.
	void tokenize();

	// Return a vector of statements.
	std::vector<std::string> getStatements();
private:
	static const std::regex DECLARE_REGEX_STMT;
	static std::string const WHITESPACES;
	const std::string& QUERY;
	std::vector<std::string> statements;
	void split();
	void validateStatement() const;
	static void isDeclareStatement(const std::string& statement);
	static void isSelectStatement(const std::string& statement);
};
