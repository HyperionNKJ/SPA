#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorSuchThatParser {
public:
	QueryPreprocessorSuchThatParser(const std::string& clause, ProcessedQuery& query);
	void parse();
private:
	enum RelRef {
		AFFECTS,
		AFFECTS_T,
		CALLS,
		CALLS_T,
		FOLLOWS,
		FOLLOWS_T,
		MODIFIES,
		NEXT,
		NEXT_T,
		PARENT,
		PARENT_T,
		USES
	};
	const std::string& CLAUSE;
	ProcessedQuery& query;
	void addClause(const RelRef& rel, const DesignEntity& paramOne, const DesignEntity& paramTwo);
	static const std::unordered_map<std::string, RelRef> STRING_TO_REL_REF;
	void isValidAffectsParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
	void isValidCallsParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
	void isValidFollowsParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
	void isValidParentParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
	void isValidModifiesParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
	void isValidNextParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
	void isValidUsesParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
};
