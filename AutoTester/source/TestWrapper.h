#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"
#include "Parser.h"
#include "PKB.h"
#include "../SPA/QueryProcessor.h"

class TestWrapper : public AbstractWrapper {
public:
	// default constructor
	TestWrapper();

	// destructor
	~TestWrapper();

	// method for parsing the SIMPLE source
	virtual void parse(std::string filename);

	// method for evaluating a query
	virtual void evaluate(std::string query, std::list<std::string>& results);
private:
	Parser parser;
	PKB pkb;
	QueryProcessor queryProcessor;
};

#endif
