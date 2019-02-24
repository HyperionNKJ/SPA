#include <string>
#include "QueryParser.h"
#include "QueryTokenizer.h"
#include "QueryPreprocessor.h"

QueryPreprocessor::QueryPreprocessor(const std::string& query) : QUERY(query) {

}

bool QueryPreprocessor::parse() {
	QueryTokenizer tokenizer = QueryTokenizer(QUERY);
	bool status = tokenizer.tokenize();

	if (!status) {
		return false;
	}

	std::vector<Statement> statements = tokenizer.getStatements();
	QueryParser parser = QueryParser(statements);
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
