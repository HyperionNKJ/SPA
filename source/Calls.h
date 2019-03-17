#pragma once
#include "Clause.h"

class Calls : public Clause {
private:
	Result* evaluateFixedSynonym(const string&, const string&); // e.g. Calls("main", p)
	Result* evaluateFixedUnderscore(const string&); // e.g. Calls("main", _)
	Result* evaluateFixedFixed(const string&, const string&); // e.g. Calls("main", "first")
	Result* evaluateSynonymSynonym(const string&, const string&); // e.g. Calls(p, q)
	Result* evaluateSynonymUnderscore(const string&); // e.g. Calls(p, _)
	Result* evaluateSynonymFixed(const string&, const string&); // Calls(p, "first")
	Result* evaluateUnderscoreSynonym(const string&); // Calls(_, q)
	Result* evaluateUnderscoreUnderscore(); // Calls(_, _)
	Result* evaluateUnderscoreFixed(const string&); // Calls(_, "first")

public:
	Calls(const DesignEntity&, const DesignEntity&);

	Result evaluate(const PKB&) override;
};
