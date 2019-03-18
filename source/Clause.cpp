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
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	unordered_set<string> synonyms;
	if (paraOneType != UNDERSCORE && paraOneType != FIXED) {
		synonyms.insert(paraOne.getValue());
	}
	if (paraTwoType != UNDERSCORE && paraTwoType != FIXED) {
		synonyms.insert(paraTwo.getValue());
	}
	return synonyms; // may return 0, 1, or 2 synonyms in a set
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
