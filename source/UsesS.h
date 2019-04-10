#pragma once
#include "Clause.h"

class UsesS : public Clause {
private:
	Result evaluateFixedVariable(const string&, const string&); // e.g. Uses(7, v)
	Result evaluateFixedUnderscore(const string&); // e.g. Uses(7, _)
	Result evaluateFixedFixed(const string&, const string&); // e.g. Uses(7, "count")
	Result evaluateSynonymVariable(const string&, const string&, const Type&); // e.g. Uses(s, v)
	Result evaluateSynonymUnderscore(const string&, const Type&); // e.g. Uses(a, _)
	Result evaluateSynonymFixed(const string&, const string&, const Type&); // e.g. Uses(w, "count")

public:
	UsesS(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};