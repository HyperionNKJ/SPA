#include <regex>
#include <sstream>
#include <string>
#include "QueryPreprocessorHelper.h"
#include "QueryPreprocessorError.h"
#include "QueryPreprocessorTokenizer.h"

constexpr auto DELIMITER = ';';

// Regular Expressions
constexpr char DECLARE_STMT[] = "^(stmt|switch|read|print|call|while|if|assign|variable|constant|prog_line|procedure) [a-zA-z][a-zA-Z0-9,]*$";
const std::regex QueryPreprocessorTokenizer::DECLARE_REGEX_STMT(DECLARE_STMT);

// Initializes a newly created QueryPreprocessorFormatterTokenizer.
QueryPreprocessorTokenizer::QueryPreprocessorTokenizer(const std::string& query)
	: QUERY(query) {}

// Tokenises the query into statements.
// Returns false if first to last statement is not a declarative statement or 
// if the last statement is a Select statement.
void QueryPreprocessorTokenizer::tokenize() {
	split();
	validateStatement();
}

// Return a vector of statements.
std::vector<std::string> QueryPreprocessorTokenizer::getStatements() {
	return statements;
}

// Splits the query into statements.
void QueryPreprocessorTokenizer::split() {
	statements = QueryPreprocessorHelper::split(QUERY, DELIMITER);
}

// Validate that the statements are in the correct order.
// First to second last statement must be Declarative statements.
// Last statement must be a Select statement.
void QueryPreprocessorTokenizer::validateStatement() const {
	// loop through the first to second last statement and check that they are
	// all declarative statements
	size_t numberOfStatements = statements.size();
	for (size_t index = 0; index < numberOfStatements - 1; index++) {
		isDeclareStatement(statements[index]);
	}

	// last statement must be a select statement
	isSelectStatement(statements.back());
}

// Check if statement is a declarative statement.
void QueryPreprocessorTokenizer::isDeclareStatement(const std::string& statement) {
	bool isValid = std::regex_match(statement, DECLARE_REGEX_STMT);
	if (!isValid) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}
}

// Check if statement is a select statement.
void QueryPreprocessorTokenizer::isSelectStatement(const std::string& statement) {
	bool isValid = statement.find("Select ", 0, 7) != std::string::npos;
	if (!isValid) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}
}
