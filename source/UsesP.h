#pragma once
#include "Clause.h"

class UsesP : public Clause {
private:
	Result* evaluateFixedVariable(const string&, const string&); // e.g. Uses("main", v)
	Result* evaluateFixedUnderscore(const string&); // e.g. Uses("main", _)
	Result* evaluateFixedFixed(const string&, const string&); // e.g. Uses("main", "count")
	Result* evaluateSynonymVariable(const string&, const string&); // e.g. Uses(p, v)
	Result* evaluateSynonymUnderscore(const string&); // e.g. Uses(p, _)
	Result* evaluateSynonymFixed(const string&, const string&); // e.g. Uses(p, "count")

public:
	UsesP(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};