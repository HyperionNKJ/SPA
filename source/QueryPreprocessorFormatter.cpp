#include <string>
#include "QueryPreprocessorError.h"
#include "QueryPreprocessorFormatter.h"

constexpr char CHAR_SPACE = ' ';
constexpr char CHAR_CLOSE_BRACKET = ')';

constexpr char STRING_SPACE[] = " ";

constexpr char WHITESPACE[] = " \t\f\v\n\r";
constexpr char LEADING_SPACE[] = "()>=,_;\"*+-/%";
constexpr char TRAILING_SPACE[] = "(<=,_;\"*+-/%";

// Initializes a newly created QueryPreprocessorFormatter.
QueryPreprocessorFormatter::QueryPreprocessorFormatter(std::string& query)
	: query(query) {
	if (query.empty()) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}

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
		if (isspace(query[oldPos++])) {
			// current character is a space 
			if (!spaceFound) {
				query[newPos++] = CHAR_SPACE;
				spaceFound = true;
			}
		} else {
			// current character is a non space
			query[newPos++] = query[oldPos - 1];
			spaceFound = false;
		}
	}

	if (newPos == orginalSize) {
		return;
	}

	query.erase(newPos);
}

void QueryPreprocessorFormatter::removeLeadingSpace() {
	size_t index = query.find_first_of(LEADING_SPACE);
	while (index != std::string::npos) {
		if (index > 3
			&& query[index] == '"'
			&& query[index - 5] == 'w'
			&& query[index - 4] == 'i'
			&& query[index - 3] == 't'
			&& query[index - 2] == 'h'
			&& query[index - 1] == ' ') {
			int i = 0;
		}
		else if (index > 6
			&& query[index] == '*'
			&& query[index - 8] == 'F'
			&& query[index - 7] == 'o'
			&& query[index - 6] == 'l'
			&& query[index - 5] == 'l'
			&& query[index - 4] == 'o'
			&& query[index - 3] == 'w'
			&& query[index - 2] == 's'
			&& query[index - 1] == ' ') {
			// do nothing
			int i = 0;
		}
		else if (index > 3
			&& query[index] == '*'
			&& query[index - 5] == 'U'
			&& query[index - 4] == 's'
			&& query[index - 3] == 'e'
			&& query[index - 2] == 's'
			&& query[index - 1] == ' ') {
			// do nothing
			int i = 0;
		}
		else if (index > 3
			&& query[index] == '*'
			&& query[index - 5] == 'N'
			&& query[index - 4] == 'e'
			&& query[index - 3] == 'x'
			&& query[index - 2] == 't'
			&& query[index - 1] == ' ') {
			// do nothing
			int i = 0;
		}
		else if (index > 4
			&& query[index] == '*'
			&& query[index - 6] == 'C'
			&& query[index - 5] == 'a'
			&& query[index - 4] == 'l'
			&& query[index - 3] == 'l'
			&& query[index - 2] == 's'
			&& query[index - 1] == ' ') {
			// do nothing
			int i = 0;
		}
		else if (index > 5
			&& query[index] == '*'
			&& query[index - 7] == 'P'
			&& query[index - 6] == 'a'
			&& query[index - 5] == 'r'
			&& query[index - 4] == 'e'
			&& query[index - 3] == 'n'
			&& query[index - 2] == 't'
			&& query[index - 1] == ' ') {
			// do nothing
			int i = 0;
		}
		else if (query[index - 1] == CHAR_SPACE) {
			query.erase(index - 1, 1);
		}
		index = query.find_first_of(LEADING_SPACE, index + 2);
	}
}

void QueryPreprocessorFormatter::removeTrailingSpace() {
	for (size_t index = query.find_first_of(TRAILING_SPACE);
		index != std::string::npos;
		index = query.find_first_of(TRAILING_SPACE, index + 1)) {
		if (query[index] == '"') {
			if (query.find(" and ", index + 1) == index + 1
				|| query.find(" such that ", index + 1) == index + 1
				|| query.find(" pattern ", index + 1) == index + 1
				|| query.find(" with ", index + 1) == index + 1) {
				continue;
			}
		}

		if (query[index + 1] == CHAR_SPACE) {
			query.erase(index + 1, 1);
		}
	}
}
