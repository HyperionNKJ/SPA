#pragma once
#include "Clause.h"

class ModifiesS : public Clause {
private:
	Result* evaluateFixedVariable(const string&, const string&); // e.g. Modifies(7, v)
	Result* evaluateFixedUnderscore(const string&); // e.g. Modifies(7, _)
	Result* evaluateFixedFixed(const string&, const string&); // e.g. Modifies(7, "count")
	Result* evaluateSynonymVariable(const string&, const string&, const Type&); // e.g. Modifies(s, v)
	Result* evaluateSynonymUnderscore(const string&, const Type&); // e.g. Modifies(a, _)
	Result* evaluateSynonymFixed(const string&, const string&, const Type&); // e.g. Modifies(w, "count")

public:
	ModifiesS(const DesignEntity&, const DesignEntity&);

	Result evaluate(const PKB&) override;
};