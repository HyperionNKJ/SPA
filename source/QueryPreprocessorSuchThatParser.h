#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorSuchThatParser {
public:
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

	QueryPreprocessorSuchThatParser(const std::string& clause, ProcessedQuery& query);
		
	bool parse();
private:
	const std::string& CLAUSE;

	ProcessedQuery& query;

	bool addClause(const RelRef& rel, const DesignEntity& paramOne, const DesignEntity& paramTwo);

	static const std::unordered_map<std::string, RelRef> STRING_TO_REL_REF;

	static bool isValidAffectsParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
	static bool isValidCallsParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
	static bool isValidFollowsParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
	static bool isValidParentParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
	static bool isValidModifiesParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
	static bool isValidNextParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
	static bool isValidUsesParam(const DesignEntity& paramOne, const DesignEntity& paramTwo);
};
