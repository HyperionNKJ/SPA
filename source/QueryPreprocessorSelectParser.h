#pragma once

#include <regex>
#include <unordered_set>
#include "ProcessedQuery.h"
#include "Type.h"
#include "Statement.h"

class QueryPreprocessorSelectParser {
public:
	static const std::regex IDENT_REGEX;
	static const std::regex INT_REGEX;
	static const std::regex REL_REF_REGEX;

	QueryPreprocessorSelectParser(const Statement& statement, ProcessedQuery& processedQuery);

	bool parse();
private:
	enum ClauseType { RESULT, SUCH_THAT, PATTERN, WITH };

	const std::string STATEMENT;

	std::vector<DesignEntity> resultClElemList;
	ProcessedQuery query;

	bool parseResultCl(std::string& resultCl);
	bool parseSuchThatCl(std::string& suchThatCl);
	bool parsePatternCl(std::string& patternCl);
	bool parseWithCl(std::string& withCl);

	bool parseElem(std::string& elem);

	DesignEntity parseParameter(std::string& parameter);
	DesignEntity parseExpresion(std::string& expression);
	DesignEntity parseAttrRef(std::string elem);

	static bool isValidSynonymAttrRefPair(Type de, AttrRef attrRef);
};
