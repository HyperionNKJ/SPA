#pragma once
#include "Clause.h"

// currently only supports 1) assign pattern, 2) RHS assumed to be sub-expression
class Pattern : public Clause {
private:
	DesignEntity subject; // subject can only be of ASSIGN synonym

	Result* evaluateVariableFixed(const string&, const string&, const string&); // e.g. a(v, _"count"_);
	Result* evaluateVariableUnderscore(const string&, const string&); // e.g. a(v, _);
	Result* evaluateUnderscoreFixed(const string&, const string&); // e.g. a(_ , _"12"_);
	Result* evaluateUnderscoreUnderscore(const string&); // e.g. a(_, _);
	Result* evaluateFixedFixed(const string&, const string&, const string&); // e.g. a("count", _"12"_);
	Result* evaluateFixedUnderscore(const string&, const string&); // e.g. a("count", _);

public:
	Pattern(const DesignEntity&, const DesignEntity&, const DesignEntity&); // In order of subject, left arg, right arg of pattern clause
	Result evaluate(const PKB&) override;
};