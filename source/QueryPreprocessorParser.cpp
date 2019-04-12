#include <regex>
#include <unordered_set>
#include <iostream>
#include "DesignEntity.h"
#include "QueryPreprocessorError.h"
#include "QueryPreprocessorHelper.h"
#include "QueryPreprocessorDeclareParser.h"
#include "QueryPreprocessorSelectParser.h"
#include "QueryPreprocessorParser.h"

// Initializes a newly created QueryPreprocessorParser.
QueryPreprocessorParser::QueryPreprocessorParser(const std::vector<std::string>& statements)
	: STATEMENTS(statements) {
	query = ProcessedQuery();
}

// Parses the statements.
void QueryPreprocessorParser::parse() {
	size_t numberOfDeclarativeStatements = STATEMENTS.size() - 1;

	if (numberOfDeclarativeStatements == 0) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}

	for (size_t index = 0; index < numberOfDeclarativeStatements; index++) {
		QueryPreprocessorDeclareParser declareParser(STATEMENTS[index], query);
		declareParser.parse();
	}

	QueryPreprocessorSelectParser selectParser(STATEMENTS.back(), query);
	selectParser.parse();
}

// Returns the processed query.
ProcessedQuery QueryPreprocessorParser::getQuery() {
	return query;
}
