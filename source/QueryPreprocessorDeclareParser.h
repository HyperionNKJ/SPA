#pragma once

#include "ProcessedQuery.h"

// Parses declarations in query.
class QueryPreprocessorDeclareParser {
public:
	// Stores the parsed information.
	ProcessedQuery& query;

	// Constructor for query preprocessor declare parser. Takes in the declare 
	// statement string literal, ProcessedQuery which stores the parsed 
	// information, and the select statement.
	QueryPreprocessorDeclareParser(const std::string& statement, ProcessedQuery& query, const std::string& selectCl);
	
	// Parses the declare statement and adds synonyms and its type to 
	// declarations stored in ProcessedQuery.
	void parse();
private:
	// Immutable statement string literal.
	const std::string& STATEMENT;

	// Immutable select clause string literal.
	const std::string& SELECT_CL;
};
