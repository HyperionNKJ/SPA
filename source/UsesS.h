#pragma once
#include "Clause.h"

class UsesS : public Clause {
private:
	Result evaluateFixedVariable(const std::string&, const std::string&); // e.g. Uses(7, v)
	Result evaluateFixedUnderscore(const std::string&); // e.g. Uses(7, _)
	Result evaluateFixedFixed(const std::string&, const std::string&); // e.g. Uses(7, "count")
	Result evaluateSynonymVariable(const std::string&, const std::string&, const Type&); // e.g. Uses(s, v)
	Result evaluateSynonymUnderscore(const std::string&, const Type&); // e.g. Uses(a, _)
	Result evaluateSynonymFixed(const std::string&, const std::string&, const Type&); // e.g. Uses(w, "count")

public:
	UsesS(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};