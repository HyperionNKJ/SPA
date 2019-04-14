#pragma once
#include "Clause.h"

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