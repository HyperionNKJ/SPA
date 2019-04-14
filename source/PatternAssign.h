#pragma once
#include "Clause.h"

class PatternAssign : public Clause {
private:
	Result evaluateVariableExact(const std::string&, const std::string&, const std::string&); // e.g. a(v, "x+y+10")
	Result evaluateVariableSub(const std::string&, const std::string&, const std::string&); // e.g. a(v, _"x+y+10"_)
	Result evaluateVariableUnderscore(const std::string&, const std::string&); // e.g. a(v, _)
	Result evaluateUnderscoreExact(const std::string&, const std::string&); // e.g. a(_, "count")
	Result evaluateUnderscoreSub(const std::string&, const std::string&); // e.g. a(_, _"count+10"_)
	Result evaluateUnderscoreUnderscore(const std::string&); // e.g. a(_, _)
	Result evaluateFixedExact(const std::string&, const std::string&, const std::string&); // e.g. a("x", "x+1")
	Result evaluateFixedSub(const std::string&, const std::string&, const std::string&); // e.g. a("x", _"y*x+z"_)
	Result evaluateFixedUnderscore(const std::string&, const std::string&); // e.g. a("i", _)

public:
	// In order of subject, left arg, right arg of assign pattern clause. Right argument must be in post-fix notation?
	PatternAssign(const DesignEntity&, const DesignEntity&, const DesignEntity&); 
	Result evaluate(const PKB&) override;
};
