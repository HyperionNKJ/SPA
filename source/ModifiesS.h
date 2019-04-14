#pragma once
#include "Clause.h"

// A class that represents ModifiesS clause and its evaluation logic
class ModifiesS : public Clause {
private:
	Result evaluateFixedVariable(const std::string&, const std::string&); // e.g. Modifies(7, v)
	Result evaluateFixedUnderscore(const std::string&); // e.g. Modifies(7, _)
	Result evaluateFixedFixed(const std::string&, const std::string&); // e.g. Modifies(7, "count")
	Result evaluateSynonymVariable(const std::string&, const std::string&, const Type&); // e.g. Modifies(s, v)
	Result evaluateSynonymUnderscore(const std::string&, const Type&); // e.g. Modifies(a, _)
	Result evaluateSynonymFixed(const std::string&, const std::string&, const Type&); // e.g. Modifies(w, "count")

public:
	ModifiesS(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};