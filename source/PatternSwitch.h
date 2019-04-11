#pragma once
#include "Clause.h"

class PatternSwitch : public Clause {
private:
	Result evaluateVariable(const string&, const string&); // e.g. sw(v, _)
	Result evaluateUnderscore(const string&); // e.g. sw(_, _)
	Result evaluateFixed(const string&, const string&); // e.g. sw("count", _)

public:
	// first arg represents the subject of clause (must be SWITCH type), second arg represents control variable (left arg)
	PatternSwitch(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};
