#pragma once
#include "DesignEntity.h"
#include "Result.h"
#include "PKB.h"

enum ClauseType {
	MODIFIES_P, MODIFIES_S, USES_P, USES_S, PARENT, PARENT_T, FOLLOWS, 
	FOLLOWS_T, CALLS, CALLS_T, NEXT, NEXT_T, AFFECTS, AFFECTS_T, 
	PATTERN_ASSIGN, PATTERN_IF, PATTERN_WHILE, PATTERN_SWITCH, WITH
};

class Clause {
protected:
	ClauseType type;
	DesignEntity paraOne;
	DesignEntity paraTwo;
	DesignEntity subject; // only for pattern clauses
	std::unordered_set<std::string> synonyms; // can have 0, 1 or 2 synonyms
	PKB pkb;

	// reduced domain values that synonym/s (if exist) can take, based on the intermediate table. Only used by computionally expensive clause: Next*
	std::unordered_map<std::string, std::unordered_set<int>> reducedDomain;

	bool Clause::isSynonym(Type&);
	void setSynonyms();

public:
	ClauseType getClauseType();
	DesignEntity getParaOne();
	DesignEntity getParaTwo();
	void setParaOne(const DesignEntity&);
	void setParaTwo(const DesignEntity&);
	std::unordered_set<std::string> getSynonyms();
	int getNumOfSynonyms();
	void setReducedDomain(const std::unordered_map<std::string, std::unordered_set<int>>&);
	virtual Result evaluate(const PKB&) = 0;
};