#pragma once
#include "ProcessedQuery.h"

// Process the query and store the parsed information in ProcessedQuery.
class QueryPreprocessor {
public:
	// Constructor for query preprocessor. Takes in the PQL string literal.
	QueryPreprocessor(std::string& query);

	// Parses the string literal of PQL query and returns ProcesedQuery containing
	// the parsed information.
	ProcessedQuery parse();
private:
	// String literal of PQL.
	std::string& query;

	// Stores the parsed information.
	ProcessedQuery processedQuery;
};
