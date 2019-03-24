#include <string>
#include "QueryPreprocessorFormatter.h"
#include "QueryPreprocessorParser.h"
#include "QueryPreprocessorTokenizer.h"
#include "QueryPreprocessor.h"

QueryPreprocessor::QueryPreprocessor(std::string& query): query(query) {

}

bool QueryPreprocessor::parse() {
	QueryPreprocessorFormatter formatter = QueryPreprocessorFormatter(query);
	query = formatter.getFormattedQuery();

	// invalid when:
	// 1. there exist a statement that is not a select or delcare statement
	// 2. select statement is not the last statement in the query
	QueryPreprocessorTokenizer tokenizer = QueryPreprocessorTokenizer(query);  
	bool status = tokenizer.tokenize();

	if (!status) {
		return false;
	}

	std::vector<std::string> statements = tokenizer.getStatements();

	QueryPreprocessorParser parser = QueryPreprocessorParser(statements);
	status = parser.parse();

	if (!status) {
		return false;
	}

	processedQuery = parser.getQuery();

	return true;
}

ProcessedQuery QueryPreprocessor::getProcessedQuery() {
	return processedQuery;
}
