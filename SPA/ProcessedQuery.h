#pragma once

#include <unordered_map>
#include <unordered_set>
#include "Clause.h"
#include "PatternClause.h"
#include "SuchThatClause.h"

class ProcessedQuery {
public:
	ProcessedQuery();

	bool insertDeclaration(const std::string& synonym,
		const Type& designEntityType);

	void addSynonym(const std::string& newSynonym);

	bool addSuchThatClause(const RelationshipType& type,
		const ParameterType& paramOneType,
		const std::string& paramOneValue,
		const ParameterType& paramTwoType,
		const std::string& paramTwoValue);

	bool addPatternClause(const DesignEntity& assign,
		const DesignEntity& paramOne,
		const DesignEntity& paramTwo);

	std::unordered_set<Clause> getClauses();
	DesignEntity getSelectedSynonym();
private:
	std::unordered_map<std::string, Type> declarations;

	bool hasSuchThatClause;
	bool hasPatternClause;

	std::string synonym;

	SuchThatClause suchThatClause;
	PatternClause patternClause;
};
