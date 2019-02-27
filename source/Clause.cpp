#include "Clause.h"

Clause::Clause(const DesignEntity& paraOne, const DesignEntity& paraTwo) {
	this->paraOne = paraOne;
	this->paraTwo = paraTwo;
}

void Clause::setParaOne(const DesignEntity& paraOne) {
	this->paraOne = paraOne;
}

void Clause::setParaTwo(const DesignEntity& paraTwo) {
	this->paraTwo = paraTwo;
}
