#pragma once
#include "Clause.h"

class PatternIf : public Clause {
private:
	DesignEntity subject; // subject can only be of IF synonym

	Result* evaluateVariable(const string&, const string&); // e.g. i(v, _, _)
	Result* evaluateUnderscore(const string&); // e.g. i(_, _, _)
	Result* evaluateFixed(const string&, const string&); // e.g. i("count", _, _)

public:
	// first arg represents the subject of clause (must be IF type), second arg represents control variable (left arg)
	PatternIf(const DesignEntity&, const DesignEntity&);
	unordered_set<string> getSynonyms() override; // overriden method from base class to include this->subject as synonym
	Result evaluate(const PKB&) override;
};
