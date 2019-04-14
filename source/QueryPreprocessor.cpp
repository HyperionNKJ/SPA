#include <string>
#include "QueryPreprocessor.h"
#include "QueryPreprocessorFormatter.h"
#include "QueryPreprocessorParser.h"
#include "QueryPreprocessorTokenizer.h"

// Constructor for query preprocessor. Takes in the PQL string literal.
QueryPreprocessor::QueryPreprocessor(std::string& query) : query(query) {}

// Parses the string literal of PQL query and returns ProcesedQuery containing
// the parsed information.
ProcessedQuery QueryPreprocessor::parse() {
	QueryPreprocessorFormatter formatter = QueryPreprocessorFormatter(query);
	query = formatter.getFormattedQuery();

	// invalid when:
	// 1. there exist a statement that is not a select or delcare statement
	// 2. select statement is not the last statement in the query
	QueryPreprocessorTokenizer tokenizer = QueryPreprocessorTokenizer(query);
	tokenizer.tokenize();

	// invalid when there exist a statement that cannot be parsed
	std::vector<std::string> statements = tokenizer.getStatements();
	QueryPreprocessorParser parser = QueryPreprocessorParser(statements);
	parser.parse();

	ProcessedQuery query = parser.getQuery();
	query.optimiseClauses();

	return query;
}