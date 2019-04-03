#include <string>
#include "QueryPreprocessorFormatter.h"

constexpr char CHAR_SPACE = ' ';
constexpr char CHAR_CLOSE_BRACKET = ')';

constexpr char STRING_SPACE[] = " ";

constexpr char WHITESPACE[] = " \t\f\v\n\r";
constexpr char FIRST_CAPTURE_GROUP[] = "$1";
constexpr char LEADING_SPACE[] = "()>=,_;\"";
constexpr char TRAILING_SPACE[] = "(<=,_;\"";

// Initializes a newly created QueryPreprocessorFormatter.
QueryPreprocessorFormatter::QueryPreprocessorFormatter(std::string& query)
	: query(query) {
	trim();
	removeExtraWhitespace();
}

// Omits the leading and trailing whitespace of the query.
void QueryPreprocessorFormatter::trim() {
	size_t start = query.find_first_not_of(WHITESPACE);
	size_t end = query.find_last_not_of(WHITESPACE) - start + 1;
	query = query.substr(start, end);
}

// Removes redundant whitespace of the query.
void QueryPreprocessorFormatter::removeExtraWhitespace() {
	removeSpaces();
	removeLeadingSpace();
	removeTrailingSpace();

	size_t hasFixRHSWithClause = query.find("\"and");
	while (hasFixRHSWithClause != std::string::npos) {
		query.insert(hasFixRHSWithClause + 1, STRING_SPACE);
		hasFixRHSWithClause = query.find("\"and");
	}
}

// Returns the formatted query.
std::string QueryPreprocessorFormatter::getFormattedQuery() {
	return query;
}

void QueryPreprocessorFormatter::removeSpaces() {
	size_t orginalSize = query.length();

	size_t newPos = 0;
	size_t oldPos = 0;

	// flag that sets to true is space is found 
	bool spaceFound = false;

	// read all characters of original string 
	while (oldPos < orginalSize) {
		if (query[oldPos] != CHAR_SPACE
			&& query[oldPos] != '\t'
			&& query[oldPos] != '\f'
			&& query[oldPos] != '\v'
			&& query[oldPos] != '\n'
			&& query[oldPos] != '\r') {
			// current characters is non-space 
			query[newPos++] = query[oldPos++];
			spaceFound = false;
		}
		else if (query[oldPos++] == CHAR_SPACE
			|| query[oldPos++] == '\t'
			|| query[oldPos++] == '\f'
			|| query[oldPos++] == '\v'
			|| query[oldPos++] == '\n'
			|| query[oldPos++] == '\r') {
			// current character is a space 
			if (!spaceFound) {
				query[newPos++] = CHAR_SPACE;
				spaceFound = true;
			}
		}
	}

	size_t spacePos = query.find_last_not_of(WHITESPACE) + 1;
	if (spacePos != std::string::npos) {
		query.erase(spacePos, orginalSize - spacePos);
	}

	if (query.back() == CHAR_CLOSE_BRACKET && query[query.size() - 2] == CHAR_CLOSE_BRACKET) {
		query.erase(query.size() - 1, 1);
	}
}

void QueryPreprocessorFormatter::removeLeadingSpace() {
	size_t index = query.find_first_of(LEADING_SPACE);
	while (index != std::string::npos) {
		if (query[index - 1] == CHAR_SPACE) {
			query.erase(index - 1, 1);
		}
		index = query.find_first_of(LEADING_SPACE, index + 2);
	}
}

void QueryPreprocessorFormatter::removeTrailingSpace() {
	size_t index = query.find_first_of(TRAILING_SPACE);
	while (index != std::string::npos) {
		if (query[index + 1] == CHAR_SPACE) {
			query.erase(index + 1, 1);
		}
		index = query.find_first_of(TRAILING_SPACE, index + 1);
	}
}
