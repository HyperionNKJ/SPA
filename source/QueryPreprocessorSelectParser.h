#pragma once

#include <regex>
#include <unordered_set>
#include "ProcessedQuery.h"
#include "Type.h"

class QueryPreprocessorSelectParser {
public:
	static const std::regex REL_REF_REGEX;

	ProcessedQuery query;

	QueryPreprocessorSelectParser(const std::string& statement, ProcessedQuery& query);

	bool parse();
private:
	enum ClauseType { RESULT, SUCH_THAT, PATTERN, WITH };

	const std::string STATEMENT;

	bool parseSuchThatCl(std::string& suchThatCl);

	DesignEntity parseParameter(std::string& parameter);
};
