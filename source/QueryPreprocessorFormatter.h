#pragma once
#include <string>

// Formats the query.
class QueryPreprocessorFormatter {
public:
	// Initializes a newly created QueryPreprocessorFormatter.
	QueryPreprocessorFormatter(std::string& query);

	// Returns the formatted query.
	std::string getFormattedQuery();
private:
	// String literal of the query.
	std::string query;

	// Omits the leading and trailing whitespace of the query.
	void trim();

	// Removes redundant whitespace of the query.
	void removeExtraWhitespace();

	// Removes extra whitespaces.
	void removeSpaces();

	// Remove leading spaces of special characters.
	void removeLeadingSpace();

	// Removes trailing whitespace of special characters.
	void removeTrailingSpace();
};