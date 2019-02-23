#pragma once
#include "Clause.h"

// consider combining Parent and ParentT into a single class with "isTransitive" as boolean.
class Parent : public Clause {
private:
	Result* evaluateFixedSynonym(const string&, const string&, const Type&); // e.g. Parent(12, w)
	Result* evaluateFixedUnderscore(const string&); // e.g. Parent(3, _)
	Result* evaluateFixedFixed(const string&, const string&); // e.g. Parent(4, 6)
	Result* evaluateSynonymSynonym(const string&, const string&, const Type&, const Type&); // e.g. Parent(i, a)
	Result* evaluateSynonymUnderscore(const string&, const Type&); // e.g. Parent(w, _)
	Result* evaluateSynonymFixed(const string&, const string&, const Type&); // Parent(i, 12)
	Result* evaluateUnderscoreSynonym(const string&, const Type&); // Parent(_, a)
	Result* evaluateUnderscoreUnderscore(); // Parent(_, _)
	Result* evaluateUnderscoreFixed(const string&); // Parent(_, 23)

public:
	Parent(const DesignEntity&, const DesignEntity&);

	Result evaluate(const PKB&) override;
};