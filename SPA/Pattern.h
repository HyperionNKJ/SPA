#pragma once
#include "Clause.h"

class Pattern : public Clause {
private:
	DesignEntity subject; // subject can only be of ASSIGN, WHILE, IF synonyms
public:
	Pattern(DesignEntity, DesignEntity, DesignEntity); // In order of subject, left arg, right arg of pattern clause

	Result evaluate(PKB) override;
};