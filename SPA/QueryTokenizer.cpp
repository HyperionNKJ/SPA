#include <regex>
#include <string>
#include <sstream>
#include "QueryPreprocessorHelper.h"
#include "Statement.h"
#include "QueryTokenizer.h"

constexpr auto DELIMITER = ';';
constexpr auto SPACE = ' ';

const std::regex QueryTokenizer::DECLARE_REGEX_STMT("^(stmt|read|print|while|if|assign|variable|constant|procedure) [a-zA-z][a-zA-Z0-9,]*$");
const std::string QueryTokenizer::WHITESPACES(" \t\f\v\n\r");

// constructor initialises query
QueryTokenizer::QueryTokenizer(const std::string& newQuery) : query(newQuery) {
	
}

// returns false if any of the statement is neither a select statement or
// declaration statement
bool QueryTokenizer::tokenize() {
	format();
	split();
	return tag();
}

// return the statements
std::vector<Statement> QueryTokenizer::getStatements() {
	return statements;
}

// removes redundant whitespaces
void QueryTokenizer::format() {
	size_t originalLength = query.length();
	size_t newIndex = 0;
	size_t oldIndex = 0;

	bool hasLeadingWhitespace = false;

	// remove leading whitespaces
	oldIndex = query.find_first_not_of(WHITESPACES);

	// redundant whitespaces
	while (oldIndex < originalLength) {
		if (!isspace(query[oldIndex])) {
			if (isNoLeadingSpaceChar(query[oldIndex])
					&& newIndex > 0
					&& isspace(query[newIndex - 1])) {
				query[newIndex - 1] = query[oldIndex];
			}
			else if (isNoPrecedingSpaceChar(query[oldIndex])
					&& oldIndex + 1 < originalLength
					&& isspace(query[oldIndex + 1])) {
				query[newIndex] = query[oldIndex];
				newIndex++;
				oldIndex++;
				while (oldIndex + 1 < originalLength
						&& isspace(query[oldIndex + 1])) {
					oldIndex++;
				}
			}
			else {
				query[newIndex] = query[oldIndex];
				newIndex++;
			}

			oldIndex++;
			hasLeadingWhitespace = false;
		}
		else {
			if (!hasLeadingWhitespace) {
				query[newIndex] = SPACE;
				hasLeadingWhitespace = true;
				newIndex++;
			}

			oldIndex++;
		}
	}

	// remove old characters and trailing whitespace
	if (!isNoPrecedingSpaceChar(query[newIndex - 1])) {
		newIndex--;
	}
	query.erase(newIndex);
}

// split into individual statements
void QueryTokenizer::split() {
	std::stringstream ss(query);
	std::string statement;
	while (std::getline(ss, statement, DELIMITER)) {
		statements.push_back(statement);
	}
}

// tag statements
bool QueryTokenizer::tag() {
	for (Statement& statement : statements) {
		StatementType statementType;

		if (isSelectStmt(statement.VALUE)) {
			statementType = StatementType::SELECT;
			
		}
		else if (isDeclareStmt(statement.VALUE)) {
			statementType = StatementType::DECLARATION;
		}
		else {
			return false;
		}

		statement.setStatementType(statementType);
	}

	return true;
}

bool QueryTokenizer::isNoLeadingSpaceChar(char& target) {
	return target == ';'
		|| target == '('
		|| target == ')'
		|| target == ','
		|| target == '"'
		|| target == '_';
}

bool QueryTokenizer::isNoPrecedingSpaceChar(char& target) {
	return target == ';'
		|| target == '('
		|| target == ','
		|| target == '"'
		|| target == '_';
}

bool QueryTokenizer::isDeclareStmt(const std::string& statement) {
	return std::regex_match(statement, DECLARE_REGEX_STMT);
}

bool QueryTokenizer::isSelectStmt(const std::string& statement) {
	return !statement.rfind("Select", 0, 6);
}
