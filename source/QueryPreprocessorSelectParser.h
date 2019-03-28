#pragma once

#include <regex>
#include <unordered_set>
#include "ProcessedQuery.h"
#include "Type.h"

class QueryPreprocessorSelectParser {
public:
	static const std::regex IDENT_REGEX;
	static const std::regex INT_REGEX;
	static const std::regex REL_REF_REGEX;

	ProcessedQuery query;

	QueryPreprocessorSelectParser(const std::string& statement, ProcessedQuery& query);

	bool parse();
private:
	enum ClauseType { RESULT, SUCH_THAT, PATTERN, WITH };

	const std::string STATEMENT;

	bool parseResultCl(std::string& resultCl);
	bool parseSuchThatCl(std::string& suchThatCl);
	bool parseWithCl(std::string& withCl);

	bool parseElem(std::string& elem);

	DesignEntity parseParameter(std::string& parameter);
	DesignEntity parseAttrRef(std::string elem);

	static bool isValidSynonymAttrRefPair(Type de, AttrRef attrRef);
};
