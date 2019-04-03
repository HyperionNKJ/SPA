#include <string>
#include "QueryPreprocessorWithParser.h"
#include "QueryPreprocessorHelper.h"
#include "With.h"

constexpr char EQUAL = '=';
constexpr char EMPTY[] = "";

// Initializes a newly created QueryPreprocessorWithParser.
QueryPreprocessorWithParser::QueryPreprocessorWithParser(std::string& withCl, ProcessedQuery& query)
	: WITH_CL(withCl), query(query) {
}

// Parses the with clause.
// Returns true if parsing is successful and false if unsucessful.
bool QueryPreprocessorWithParser::parse() {
	// with clauses must have "="
	size_t lhsSize = WITH_CL.find(EQUAL);
	if (lhsSize == std::string::npos) {
		return false;
	}

	// extract the lhs and rhs of with clause
	std::string lhs = WITH_CL.substr(0, lhsSize);
	std::string rhs = WITH_CL.substr(lhsSize + 1);
	DesignEntity paramOne = parseWithParam(lhs);
	DesignEntity paramTwo = parseWithParam(rhs);

	if (paramOne.isInvalid() || paramTwo.isInvalid()) {
		return false;
	}

	// LHS AttrRef type must be the same as RHS AttrRef type
	bool status = isValidAttrRefComparator(paramOne, paramTwo);
	if (!status) {
		return false;
	}

	With* withClause = new With(paramOne, paramTwo);
	query.addWithClause(withClause, WITH_CL);

	return true;
}

// Parses the individual parameters in with clause.
DesignEntity QueryPreprocessorWithParser::parseWithParam(const std::string& paramString) const {
	DesignEntity param = QueryPreprocessorHelper::getParam(paramString, query);

	if (param.isType(Type::UNDERSCORE)) {
		return DesignEntity(EMPTY, Type::INVALID);
	}

	if (param.isType(Type::FIXED)) {
		std::string& val = param.getValue();
		if (QueryPreprocessorHelper::isInt(val)) {
			param.setType(Type::WITH_INTEGER);
		}
		else {
			param.setType(Type::WITH_STRING);
		}
	}

	std::vector<Type> validTypes = { Type::WITH_INTEGER, Type::WITH_STRING, Type::PROGLINE };
	if (!param.isAttrRef(AttrRef::UNASSIGNED) || param.isAnyType(validTypes)) {
		return param;
	}

	return DesignEntity(EMPTY, Type::INVALID);
}

// Validates that the comparation of two parameters is legal.
// Returns true if comparing the left and right hand side of the assignment is allowed.
bool QueryPreprocessorWithParser::isValidAttrRefComparator(const DesignEntity& paramOne, const DesignEntity& paramTwo) const {
	bool isNameAttrRefOne = false;
	bool isNameAttrRefTwo = false;

	std::vector<AttrRef> legalAttrRefs = {AttrRef::PROC_NAME, AttrRef::VAR_NAME};

	if (paramOne.isAnyAttrRef(legalAttrRefs) || paramOne.isType(Type::WITH_STRING)) {
		isNameAttrRefOne = true;
	}

	if (paramTwo.isAnyAttrRef(legalAttrRefs) || paramTwo.isType(Type::WITH_STRING)) {
		isNameAttrRefTwo = true;
	}

	return isNameAttrRefOne == isNameAttrRefTwo;
}
