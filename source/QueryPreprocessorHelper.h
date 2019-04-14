#pragma once
#include <unordered_map>
#include <unordered_set>
#include "ProcessedQuery.h"
#include "AttrRef.h"
#include "Type.h"

// Helper class for parsing queries.
class QueryPreprocessorHelper {
public:
	// Splits this string around matches of the given delimiter.
	static std::vector<std::string> split(const std::string& target, const char& delimiter);
	
	// Returns the type enum.
	static Type getType(const std::string& typeString);

	// Returns the attribute reference enum.
	static AttrRef getAttrRef(const std::string& attrRefString);
	
	// Returns the postfix of the given infix expression.
	static std::string getPostFix(const std::string& infix);
	
	// Returns true if target is an integer.
	static bool isInt(const std::string& target);

	// Returns true if target is a variable.
	static bool isVar(const std::string& target);

	//Returns the element of the parameter.
	static DesignEntity getParam(const std::string& param, ProcessedQuery& query);
private:
	static const std::unordered_map<std::string, AttrRef> STRING_TO_ATTR_REF;
	static const std::unordered_map<std::string, Type> STRING_TO_TYPE;
	static const std::unordered_map<Type, std::string> TYPE_TO_STRING;
};
