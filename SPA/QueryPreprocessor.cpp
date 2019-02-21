// QueryPreprocessor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <chrono>
#include "QueryParser.h"
#include "QueryTokenizer.h"
#include "QueryPreprocessor.h"

int main() {
	auto start = std::chrono::high_resolution_clock::now();
	std::string rawQuery = "assign a; stmt b,b,   c;Select a such that Follows (1,s) pattern a (\"x\",_\"y\"_);     ";

	QueryTokenizer tokenizer = QueryTokenizer(rawQuery);
	bool status = tokenizer.tokenize();

	if (!status) {
		std::cout << status;
		std::cout << "There exist a statement that is neither a select statement or declarative statement." << std::endl;
		return 0;
	}

	std::vector<Statement> statements = tokenizer.getStatements();
	QueryParser parser = QueryParser(statements);
	ProcessedQuery query = parser.getQuery();
	query.print();

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << duration.count() << std::endl;
}

QueryPreprocessor::QueryPreprocessor(const std::string& query) : QUERY(query) {

}

bool QueryPreprocessor::parse() {
	return tokenize();
}

bool QueryPreprocessor::tokenize() {
	return false;
}

bool QueryPreprocessor::validateSyntax() {
	return false;
}
