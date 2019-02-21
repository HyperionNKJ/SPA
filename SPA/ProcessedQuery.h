#pragma once

#include <unordered_map>
#include "PatternClause.h"
#include "SuchThatClause.h"
#include "DesignEntityType.h"

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

	bool addPatternClause(const std::string& newSynonym,
		const ParameterType& paramOneType,
		const std::string& paramOneValue,
		const ParameterType& paramTwoType,
		const std::string& paramTwoValue);

	void print();
private:
	std::unordered_map<std::string, Type> declarations;

	bool hasSuchThatClause;
	bool hasPatternClause;

	std::string synonym;

	SuchThatClause suchThatClause;
	PatternClause patternClause;
};
