#include <string>
#include <regex>
#include "QueryPreprocessorFormatter.h"

constexpr char WHITESPACE[] = " \t\f\v\n\r";
constexpr char SPACE[] = " ";
constexpr char FIRST_CAPTURE_GROUP[] = "$1";

// Regular Expressions.
const std::regex QueryPreprocessorFormatter::LEADING_SPACE(" ([()>=,_;\"])");
const std::regex QueryPreprocessorFormatter::TRAILING_SPACE("([(<=,_;\"]) ");

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
	removeSpaces(query);
	query = std::regex_replace(query, LEADING_SPACE, FIRST_CAPTURE_GROUP);
	query = std::regex_replace(query, TRAILING_SPACE, FIRST_CAPTURE_GROUP);
}

// Returns the formatted query.
std::string QueryPreprocessorFormatter::getFormattedQuery() {
	return query;
}

void QueryPreprocessorFormatter::removeSpaces(string& original) {
	size_t orginalSize = original.length();

	size_t newPos = 0;
	size_t oldPos = 0;

	// flag that sets to true is space is found 
	bool spaceFound = false;

	// read all characters of original string 
	while (oldPos < orginalSize) {
		if (original[oldPos] != ' ' 
			&& original[oldPos] != '\t'
			&& original[oldPos] != '\f'
			&& original[oldPos] != '\v'
			&& original[oldPos] != '\n'
			&& original[oldPos] != '\r') {
			// current characters is non-space 
			original[newPos++] = original[oldPos++];
			spaceFound = false;
		}
		else if (original[oldPos++] == ' '
			|| original[oldPos++] == '\t'
			|| original[oldPos++] == '\f'
			|| original[oldPos++] == '\v'
			|| original[oldPos++] == '\n'
			|| original[oldPos++] == '\r') {
			// current character is a space 
			if (!spaceFound) {
				original[newPos++] = ' ';
				spaceFound = true;
			}
		}
	}

	size_t spacePos = original.find_last_not_of(WHITESPACE) + 1;
	if (spacePos != std::string::npos) {
		original.erase(spacePos, orginalSize - spacePos);
	}
}
