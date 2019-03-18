#pragma once
#include <string>
#include <list>
#include "DesignEntity.h"
#include "Result.h"
#include "PKB.h"

using namespace std;

enum ClauseType {
	MODIFIES_P, MODIFIES_S, USES_P, USES_S, PARENT, PARENT_T, FOLLOWS, 
	FOLLOWS_T, CALLS, CALLS_T, NEXT, NEXT_T, AFFECTS, AFFECTS_T, 
	PATTERN_ASSIGN, PATTERN_IF, PATTERN_WHILE, WITH
};

class Clause {
protected:
	ClauseType type;
	DesignEntity paraOne;
	DesignEntity paraTwo;
	PKB pkb;

	// reduced domain values that synonym/s (if exist) can take, based on the intermediate table. Only used by computionally expensive clause: Next*
	unordered_map<string, unordered_set<int>> reducedDomain; 

	Clause(const DesignEntity&, const DesignEntity&, const ClauseType&);

public:
	ClauseType getClauseType();
	void setParaOne(const DesignEntity&);
	void setParaTwo(const DesignEntity&);
	unordered_set<string> getSynonyms(); // may return 0, 1 or 2 synonyms
	void setReducedDomain(const unordered_map<string, unordered_set<int>>&);
	virtual Result evaluate(const PKB&) = 0;
};