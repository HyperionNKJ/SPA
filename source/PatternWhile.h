#pragma once
#include "Clause.h"

class PatternWhile : public Clause {
private:
	DesignEntity subject; // subject can only be of WHILE synonym

	Result* evaluateVariable(const string&, const string&); // e.g. w(v, _)
	Result* evaluateUnderscore(const string&); // e.g. w(_, _)
	Result* evaluateFixed(const string&, const string&); // e.g. w("count", _)

public:
	// first arg represents the subject of clause (must be WHILE type), second arg represents control variable (left arg)
	PatternWhile(const DesignEntity&, const DesignEntity&);
	unordered_set<string> getSynonyms() override; // overriden method from base class to include this->subject as synonym
	Result evaluate(const PKB&) override;
};
