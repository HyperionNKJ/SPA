#pragma once

#include "ProcessedQuery.h"

class QueryPreprocessorSuchThatParser {
public:
	ProcessedQuery query;

	QueryPreprocessorSuchThatParser(const std::string& clause, ProcessedQuery& query);

	bool parse();
private:

	const std::string CLAUSE;

	static bool isRelRef(std::string& relRef);
	static bool isValidAffectsParam(DesignEntity& paramOne, DesignEntity& paramTwo);
	static bool isValidCallsParam(DesignEntity& paramOne, DesignEntity& paramTwo);
	static bool isValidFollowsParam(DesignEntity& paramOne, DesignEntity& paramTwo);
	static bool isValidParentParam(DesignEntity& paramOne, DesignEntity& paramTwo);
	static bool isValidModifiesParam(DesignEntity& paramOne, DesignEntity& paramTwo);
	static bool isValidNextParam(DesignEntity& paramOne, DesignEntity& paramTwo);
	static bool isValidUsesParam(DesignEntity& paramOne, DesignEntity& paramTwo);
};
