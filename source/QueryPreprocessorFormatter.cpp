#include <string>
#include <regex>
#include "QueryPreprocessorFormatter.h"

constexpr char WHITESPACE[] = " \t\f\v\n\r";

const std::regex QueryPreprocessorFormatter::EXTRA_WHITESPACE("\\s+");
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
	query = std::regex_replace(query, EXTRA_WHITESPACE, " ");
	query = std::regex_replace(query, LEADING_SPACE, "$1");
	query = std::regex_replace(query, TRAILING_SPACE, "$1");
}

// Returns the formatted query.
std::string QueryPreprocessorFormatter::getFormattedQuery() {
	return query;
}
