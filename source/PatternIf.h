#pragma once
#include "Clause.h"

class PatternIf : public Clause {
private:
	Result evaluateVariable(const std::string&, const std::string&); // e.g. i(v, _, _)
	Result evaluateUnderscore(const std::string&); // e.g. i(_, _, _)
	Result evaluateFixed(const std::string&, const std::string&); // e.g. i("count", _, _)

public:
	// first arg represents the subject of clause (must be IF type), second arg represents control variable (left arg)
	PatternIf(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};
