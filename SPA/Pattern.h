#pragma once
#include "Clause.h"

// currently only supports 1) assign pattern, 2) RHS assumed to be sub-expression
class Pattern : public Clause {
private:
	DesignEntity subject; // subject can only be of ASSIGN synonym

	Result* evaluateVariableFixed(string, string, string); // e.g. a(v, _"count"_);
	Result* evaluateVariableUnderscore(string, string); // e.g. a(v, _);
	Result* evaluateUnderscoreFixed(string, string); // e.g. a(_ , _"12"_);
	Result* evaluateUnderscoreUnderscore(string); // e.g. a(_, _);
	Result* evaluateFixedFixed(string, string, string); // e.g. a("count", _"12"_);
	Result* evaluateFixedUnderscore(string, string); // e.g. a("count", _);

public:
	Pattern(DesignEntity, DesignEntity, DesignEntity); // In order of subject, left arg, right arg of pattern clause

	Result evaluate(PKB) override;
};