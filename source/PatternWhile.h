#pragma once
#include "Clause.h"

// A class that represents PatternWhile clause and its evaluation logic
class PatternWhile : public Clause {
private:
	Result evaluateVariable(const std::string&, const std::string&); // e.g. w(v, _)
	Result evaluateUnderscore(const std::string&); // e.g. w(_, _)
	Result evaluateFixed(const std::string&, const std::string&); // e.g. w("count", _)

public:
	// first arg represents the subject of clause (must be WHILE type), second arg represents control variable (left arg)
	PatternWhile(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};
