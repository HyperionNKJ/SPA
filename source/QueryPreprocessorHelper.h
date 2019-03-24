#pragma once

#include <unordered_map>
#include <unordered_set>
#include "AttrRef.h"
#include "Type.h"

class QueryPreprocessorHelper {
public:
	static const std::unordered_map<std::string, Type> STRING_TO_TYPE;
	static const std::unordered_map<std::string, AttrRef> STRING_TO_ATTR_REF;
	static const std::unordered_set<std::string> RESERVED_WORD;

	QueryPreprocessorHelper();
	static std::vector<std::string> split(const std::string& target, const char& delimiter);
	static Type getType(const std::string& typeString);
	static AttrRef getAttrRef(const std::string& attrRefString);
	static bool isReservedWord(const std::string& target);
	static std::string getPostFix(const std::string& infix);
};
