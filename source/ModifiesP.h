#pragma once
#include "Clause.h"

class ModifiesP : public Clause {
private:
	Result* evaluateFixedVariable(const string&, const string&); // e.g. Modifies("main", v)
	Result* evaluateFixedUnderscore(const string&); // e.g. Modifies("main", _)
	Result* evaluateFixedFixed(const string&, const string&); // e.g. Modifies("main", "count")
	Result* evaluateSynonymVariable(const string&, const string&); // e.g. Modifies(p, v)
	Result* evaluateSynonymUnderscore(const string&); // e.g. Modifies(p, _)
	Result* evaluateSynonymFixed(const string&, const string&); // e.g. Modifies(p, "count")

public:
	ModifiesP(const DesignEntity&, const DesignEntity&);

	Result evaluate(const PKB&) override;
};