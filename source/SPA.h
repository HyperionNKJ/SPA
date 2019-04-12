#pragma once

#include "Parser.h"
#include "PKB.h"
#include "QueryProcessor.h"

class SPA {
public:
	// default constructor
	SPA();

	// destructor
	~SPA();

	// method for parsing the SIMPLE source
	int parse(std::string filename);

	// method for evaluating a query
	void evaluate(std::string query, std::list<std::string>& results);
private:
	Parser *parser;
	PKB *pkb;
	QueryProcessor *queryProcessor;
};