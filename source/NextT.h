#pragma once
#include "Clause.h"

class NextT : public Clause {
private:
	Result* evaluateFixedSynonym(const string&, const string&, const Type&); // e.g. Next*(12, a)
	Result* evaluateFixedUnderscore(const string&); // e.g. Next*(12, _)
	Result* evaluateFixedFixed(const string&, const string&); // e.g. Next*(12, 13)
	Result* evaluateSynonymSynonym(const string&, const string&, const Type&, const Type&); // e.g. Next*(s, s1)
	Result* evaluateSynonymUnderscore(const string&, const Type&); // e.g. Next*(w, _)
	Result* evaluateSynonymFixed(const string&, const string&, const Type&); // Next*(s, 14)
	Result* evaluateUnderscoreSynonym(const string&, const Type&); // Next*(_, pl)
	Result* evaluateUnderscoreUnderscore(); // Next*(_, _)
	Result* evaluateUnderscoreFixed(const string&); // Next*(_, 23)

	bool evaluateSynSynFromPrev(const unordered_set<int>&, const Type&, PKB&, unordered_map<int, unordered_set<int>>&);
	bool evaluateSynSynFromNext(const unordered_set<int>&, const Type&, PKB&, unordered_map<int, unordered_set<int>>&);

public:
	NextT(const DesignEntity&, const DesignEntity&);
	Result evaluate(const PKB&) override;
};