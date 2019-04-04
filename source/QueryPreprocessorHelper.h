#pragma once

#include <unordered_map>
#include <unordered_set>
#include "ProcessedQuery.h"
#include "AttrRef.h"
#include "Type.h"

class QueryPreprocessorHelper {
public:
	static std::vector<std::string> split(const std::string& target, const char& delimiter);
	static Type getType(const std::string& typeString);
	static AttrRef getAttrRef(const std::string& attrRefString);
	static std::string getPostFix(const std::string& infix);
	static bool isInt(const std::string& target);
	static bool isVar(const std::string& target);
	static DesignEntity getParam(const std::string& param, ProcessedQuery& query);
private:
	static const std::unordered_map<std::string, AttrRef> STRING_TO_ATTR_REF;
	static const std::unordered_map<std::string, Type> STRING_TO_TYPE;
	static const std::unordered_map<Type, std::string> TYPE_TO_STRING;
};
