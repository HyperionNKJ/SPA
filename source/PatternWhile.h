#pragma once
#include "Clause.h"

class PatternWhile : public Clause {
private:
	Result* evaluateVariable(const string&, const string&); // e.g. w(v, _)
	Result* evaluateUnderscore(const string&); // e.g. w(_, _)
	Result* evaluateFixed(const string&, const string&); // e.g. w("count", _)

public:
	// first arg represents the subject of clause (must be WHILE type), second arg represents control variable (left arg)
	PatternWhile(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};
