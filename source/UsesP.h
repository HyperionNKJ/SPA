#pragma once
#include "Clause.h"

// A class that represents UsesP clause and its evaluation logic
class UsesP : public Clause {
private:
	Result evaluateFixedVariable(const std::string&, const std::string&); // e.g. Uses("main", v)
	Result evaluateFixedUnderscore(const std::string&); // e.g. Uses("main", _)
	Result evaluateFixedFixed(const std::string&, const std::string&); // e.g. Uses("main", "count")
	Result evaluateSynonymVariable(const std::string&, const std::string&); // e.g. Uses(p, v)
	Result evaluateSynonymUnderscore(const std::string&); // e.g. Uses(p, _)
	Result evaluateSynonymFixed(const std::string&, const std::string&); // e.g. Uses(p, "count")

public:
	UsesP(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};