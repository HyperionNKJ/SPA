#include <regex>
#include <sstream>
#include <string>
#include "QueryPreprocessorTokenizer.h"

constexpr auto DELIMITER = ';';

// Regular Expressions
constexpr char DECLARE_STMT[] = "^(stmt|read|print|call|while|if|assign|variable|constant|prog_line|procedure) [a-zA-z][a-zA-Z0-9,]*$";
const std::regex QueryPreprocessorTokenizer::DECLARE_REGEX_STMT(DECLARE_STMT);

// Initializes a newly created QueryPreprocessorFormatterTokenizer.
QueryPreprocessorTokenizer::QueryPreprocessorTokenizer(const std::string& query)
	: query(query) {
}

// Tokenises the query into statements.
// Returns false if first to last statement is not a declarative statement or 
// if the last statement is a Select statement.
bool QueryPreprocessorTokenizer::tokenize() {
	split();
	return validateStatement();
}

// Return a vector of statements.
std::vector<std::string> QueryPreprocessorTokenizer::getStatements() {
	return statements;
}

// Splits the query into statements.
void QueryPreprocessorTokenizer::split() {
	std::stringstream ss(query);
	std::string statement;

	while (std::getline(ss, statement, DELIMITER)) {
		statements.push_back(statement);
	}
}

// Validate that the statements are in the correct order.
// First to second last statement must be Declarative statements.
// Last statement must be a Select statement.
bool QueryPreprocessorTokenizer::validateStatement() {
	// loop through the first to second last statement and check that they are
	// all declarative statements
	size_t numberOfStatements = statements.size();
	for (size_t index = 0; index < numberOfStatements - 1; index++) {
		if (!isDeclareStatement(statements[index])) {
			return false;
		}
	}

	// last statement must be a declarative statement
	if (!isSelectStatement(statements.back())) {
		return false;
	}

	return true;
}

// Check if statement is a declarative statement.
bool QueryPreprocessorTokenizer::isDeclareStatement(const std::string& statement) {
	return std::regex_match(statement, DECLARE_REGEX_STMT);
}

// Check if statement is a select statement.
bool QueryPreprocessorTokenizer::isSelectStatement(const std::string& statement) {
	return statement.find("Select ", 0, 7) != std::string::npos;
}
