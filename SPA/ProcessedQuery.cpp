#include <iostream>
#include <regex>
#include "DesignEntityTypeFactory.h"
#include "ParameterType.h"
#include "QueryPreprocessorHelper.h"
#include "ProcessedQuery.h"

ProcessedQuery::ProcessedQuery() {

}

bool ProcessedQuery::insertDeclaration(const std::string& synonym,
		const Type& type) {
	return declarations.insert({ synonym, type }).second;
}

void ProcessedQuery::addSynonym(const std::string& newSynonym) {
	synonym = newSynonym;
}

bool ProcessedQuery::addSuchThatClause(const RelationshipType& type,
		const ParameterType& paramOneType,
		const std::string& paramOneValue,
		const ParameterType& paramTwoType,
		const std::string& paramTwoValue) {
	if (hasSuchThatClause) {
		return false;
	}

	hasSuchThatClause = true;

	suchThatClause.type = type;
	suchThatClause.paramOneType = paramOneType;
	suchThatClause.paramTwoType = paramTwoType;
	suchThatClause.paramOneValue = paramOneValue;
	suchThatClause.paramTwoValue = paramTwoValue;

	return true;
}

bool ProcessedQuery::addPatternClause(const std::string& newSynonym,
		const ParameterType& paramOneType,
		const std::string& paramOneValue,
		const ParameterType& paramTwoType,
		const std::string& paramTwoValue) {
	if (hasPatternClause) {
		return false;
	}

	hasPatternClause = true;

	patternClause.synonym = newSynonym;

	patternClause.paramOneType = paramOneType;
	patternClause.paramTwoType = paramTwoType;

	patternClause.paramOneValue = paramOneValue;
	patternClause.paramTwoValue = paramTwoValue;

	return true;
}

void ProcessedQuery::print() {
	// print all declarations
	std::cout << "========== Declarations ==========" << std::endl;
	for (auto& node : declarations) {
		node;
	}

	// print synonyms
	std::cout << "========== Synonym ==========" << std::endl;
	std::cout << synonym << std::endl;

	suchThatClause;
	patternClause;
}
