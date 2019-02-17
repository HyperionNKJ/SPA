#include "Clause.h"

Clause::Clause(DesignEntity paraOne, DesignEntity paraTwo) {
	this->paraOne = paraOne;
	this->paraTwo = paraTwo;
}

void Clause::setParaOne(DesignEntity paraOne) {
	this->paraOne = paraOne;
}

void Clause::setParaTwo(DesignEntity paraTwo) {
	this->paraTwo = paraTwo;
}
