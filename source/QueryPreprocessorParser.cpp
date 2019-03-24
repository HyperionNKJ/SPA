#include <regex>
#include <unordered_set>
#include "DesignEntity.h"
#include "QueryPreprocessorHelper.h"
#include "QueryPreprocessorDeclareParser.h"
#include "QueryPreprocessorSelectParser.h"
#include "QueryPreprocessorParser.h"

// Initializes a newly created QueryPreprocessorParser.
QueryPreprocessorParser::QueryPreprocessorParser(const std::vector<std::string>& statements)
		: STATEMENTS(statements) {
}

// Parses the statements.
bool QueryPreprocessorParser::parse() {
	size_t numberOfDeclarativeStatements = STATEMENTS.size() - 1;
	for (size_t index = 0; index < numberOfDeclarativeStatements; index++) {
		QueryPreprocessorDeclareParser declareParser(STATEMENTS[index], query);
		bool status = declareParser.parse();
		if (!status) {
			return false;
		}
	}

	QueryPreprocessorSelectParser selectParser(STATEMENTS.back(), query);
	bool status = selectParser.parse();

	if (!status) {
		return false;
	}

	return true;
}

// Returns the processed query.
ProcessedQuery QueryPreprocessorParser::getQuery() {
	return query;
}
