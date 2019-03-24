#include "Clause.h"

Clause::Clause(const DesignEntity& paraOne, const DesignEntity& paraTwo, const ClauseType& type) {
	this->paraOne = paraOne;
	this->paraTwo = paraTwo;
	this->type = type;
}

ClauseType Clause::getClauseType() {
	return this->type;
}

unordered_set<string> Clause::getSynonyms() {
	unordered_set<string> synonyms;
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();

	if (isSynonym(paraOneType)) {
		synonyms.insert(paraOne.getValue());
	}
	if (isSynonym(paraTwoType)) {
		synonyms.insert(paraTwo.getValue());
	}
	return synonyms; // may return 0, 1, or 2 synonyms in a set
}

bool Clause::isSynonym(Type& type) {
	return (type == STATEMENT || type == PROGLINE || type == READ || type == PRINT || type == CALL || type == WHILE || type == IF 
		|| type == ASSIGN || type == VARIABLE || type == CONSTANT || type == PROCEDURE);
}

void Clause::setReducedDomain(const unordered_map<string, unordered_set<int>>& reducedDomain) {
	this->reducedDomain = reducedDomain;
}


void Clause::setParaOne(const DesignEntity& paraOne) {
	this->paraOne = paraOne;
}

void Clause::setParaTwo(const DesignEntity& paraTwo) {
	this->paraTwo = paraTwo;
}
