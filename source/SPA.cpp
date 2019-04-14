#include <string>
#include <iostream>
#include <list>
#include "SPA.h"

SPA::SPA() {
	parser = new Parser();
	pkb = new PKB();
	queryProcessor = new QueryProcessor();
}

SPA::~SPA() {
	delete parser;
	delete pkb;
	delete queryProcessor;
}

int SPA::parse(std::string filename) {
	int result = parser->parse(filename, *pkb);
	if (result != 0) {
		std::cout << parser->getErrorMessage() << std::endl;
		exit(1);
	}
}

// method to evaluating a query
void SPA::evaluate(std::string query, std::list<std::string>& results) {
	results = queryProcessor->evaluate(query, *pkb);
}