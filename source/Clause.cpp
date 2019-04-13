#include "Clause.h"

ClauseType Clause::getClauseType() {
	return this->type;
}

void Clause::setSynonyms() {
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();

	if (isSynonym(paraOneType)) {
		synonyms.insert(paraOne.getValue());
	}
	if (isSynonym(paraTwoType)) {
		synonyms.insert(paraTwo.getValue());
	}
	if (type == ClauseType::PATTERN_ASSIGN || type == ClauseType::PATTERN_IF || type == ClauseType::PATTERN_WHILE || type == ClauseType::PATTERN_SWITCH) {
		synonyms.insert(subject.getValue()); // subject is always a synonym
	}
}

bool Clause::isSynonym(Type& type) {
	return (type == STATEMENT || type == PROGLINE || type == READ || type == PRINT || type == CALL || type == WHILE || type == IF 
		|| type == ASSIGN || type == VARIABLE || type == CONSTANT || type == PROCEDURE || type == SWITCH);
}

std::unordered_set<std::string> Clause::getSynonyms() {
	return this->synonyms;
}

int Clause::getNumOfSynonyms() {
	return this->synonyms.size();
}

void Clause::setReducedDomain(const std::unordered_map<std::string, std::unordered_set<int>>& reducedDomain) {
	this->reducedDomain = reducedDomain;
}

DesignEntity Clause::getParaOne() {
	return this->paraOne;
}

DesignEntity Clause::getParaTwo() {
	return this->paraTwo;
}

void Clause::setParaOne(const DesignEntity& paraOne) {
	this->paraOne = paraOne;
}

void Clause::setParaTwo(const DesignEntity& paraTwo) {
	this->paraTwo = paraTwo;
}
