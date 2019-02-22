#include "Clause.h"
#include "ModifiesS.h"
#include "Pattern.h"
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

bool ProcessedQuery::addPatternClause(const DesignEntity& assign,
	const DesignEntity& paramOne,
	const DesignEntity& paramTwo) {
	if (hasPatternClause) {
		return false;
	}

	hasPatternClause = true;

	patternClause.assign = assign;
	patternClause.paramOne = paramOne;
	patternClause.paramTwo = paramTwo;

	return true;
}

std::unordered_set<Clause> ProcessedQuery::getClauses() {
	unordered_set<Clause> clauses;
	if (hasSuchThatClause) {
		if (suchThatClause.type == RelationshipType::MODIFIES_S) {
			
		}
		
	}

	if (hasPatternClause) {
		Pattern pattern(patternClause.assign, patternClause.paramOne, patternClause.paramTwo);
		clauses.insert(pattern);
	}

	return clauses;
}

DesignEntity ProcessedQuery::getSelectedSynonym() {
	return DesignEntity(synonym, declarations[synonym]);
}