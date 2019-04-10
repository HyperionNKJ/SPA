#pragma once
#include "Clause.h"

class PatternAssign : public Clause {
private:
	Result evaluateVariableExact(const string&, const string&, const string&); // e.g. a(v, "x+y+10")
	Result evaluateVariableSub(const string&, const string&, const string&); // e.g. a(v, _"x+y+10"_)
	Result evaluateVariableUnderscore(const string&, const string&); // e.g. a(v, _)
	Result evaluateUnderscoreExact(const string&, const string&); // e.g. a(_, "count")
	Result evaluateUnderscoreSub(const string&, const string&); // e.g. a(_, _"count+10"_)
	Result evaluateUnderscoreUnderscore(const string&); // e.g. a(_, _)
	Result evaluateFixedExact(const string&, const string&, const string&); // e.g. a("x", "x+1")
	Result evaluateFixedSub(const string&, const string&, const string&); // e.g. a("x", _"y*x+z"_)
	Result evaluateFixedUnderscore(const string&, const string&); // e.g. a("i", _)

public:
	// In order of subject, left arg, right arg of assign pattern clause. Right argument must be in post-fix notation?
	PatternAssign(const DesignEntity&, const DesignEntity&, const DesignEntity&); 
	Result evaluate(const PKB&) override;
};
