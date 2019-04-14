#pragma once
#include "Clause.h"

// A class that represents ModifiesP clause and its evaluation logic
class ModifiesP : public Clause {
private:
	Result evaluateFixedVariable(const std::string&, const std::string&); // e.g. Modifies("main", v)
	Result evaluateFixedUnderscore(const std::string&); // e.g. Modifies("main", _)
	Result evaluateFixedFixed(const std::string&, const std::string&); // e.g. Modifies("main", "count")
	Result evaluateSynonymVariable(const std::string&, const std::string&); // e.g. Modifies(p, v)
	Result evaluateSynonymUnderscore(const std::string&); // e.g. Modifies(p, _)
	Result evaluateSynonymFixed(const std::string&, const std::string&); // e.g. Modifies(p, "count")

public:
	ModifiesP(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};