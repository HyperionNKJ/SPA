#pragma once

#include <unordered_map>
#include <unordered_set>
#include "Clause.h"
#include "PatternClause.h"
#include "SuchThatClause.h"

class ProcessedQuery {
public:
	std::unordered_map<std::string, Type> declarations;
	SuchThatClause suchThatClause;
	PatternClause patternClause;
	std::string synonym;
	bool hasSuchThatClause;
	bool hasPatternClause;

	ProcessedQuery();

	bool insertDeclaration(const std::string& synonym,
		const Type& designEntityType);

	void addSynonym(const std::string& newSynonym);

	bool addSuchThatClause(const RelationshipType& type,
		const DesignEntity& paramOne,
		const DesignEntity& paramTwo);

	bool addPatternClause(const DesignEntity& assign,
		const DesignEntity& paramOne,
		const DesignEntity& paramTwo);

	std::unordered_set<Clause*> getClauses();
	DesignEntity getSelectedSynonym();
};
