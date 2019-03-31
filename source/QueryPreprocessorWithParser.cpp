#include <string>
#include "QueryPreprocessorWithParser.h"
#include "QueryPreprocessorHelper.h"
#include "With.h"

constexpr auto EQUAL = '=';

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

	if (paramOne.getType() == Type::INVALID || paramTwo.getType() == Type::INVALID) {
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
DesignEntity QueryPreprocessorWithParser::parseWithParam(std::string& paramString) {
	DesignEntity param = QueryPreprocessorHelper::getParam(paramString, query);

	if (param.getType() == Type::UNDERSCORE) {
		return DesignEntity("", Type::INVALID);
	}

	if (param.getType() == Type::FIXED) {
		std::string& val = param.getValue();
		if (QueryPreprocessorHelper::isInt(val)) {
			param.setType(Type::WITH_INTEGER);
		}
		else {
			param.setType(Type::WITH_STRING);
		}
	}

	if (param.getAttrRef() == AttrRef::UNASSIGNED
		&& param.getType() != Type::WITH_INTEGER
		&& param.getType() != Type::WITH_STRING
		&& param.getType() != Type::PROGLINE) {
		return DesignEntity("", Type::INVALID);
	}

	return param;
}

// Validates that the comparation of two parameters is legal.
// Returns true if comparing the left and right hand side of the assignment is allowed.
bool QueryPreprocessorWithParser::isValidAttrRefComparator(DesignEntity& paramOne, DesignEntity& paramTwo) {
	AttrRef attrRefOne = paramOne.getAttrRef();
	AttrRef attrRefTwo = paramTwo.getAttrRef();

	bool isNameAttrRefOne = false;
	bool isNameAttrRefTwo = false;

	if (attrRefOne == AttrRef::PROC_NAME
		|| attrRefOne == AttrRef::VAR_NAME
		|| paramOne.getType() == Type::WITH_STRING) {
		isNameAttrRefOne = true;
	}

	if (attrRefTwo == AttrRef::PROC_NAME
		|| attrRefTwo == AttrRef::VAR_NAME
		|| paramTwo.getType() == Type::WITH_STRING) {
		isNameAttrRefTwo = true;
	}

	if (isNameAttrRefOne != isNameAttrRefTwo) {
		return false;
	}

	return true;
}
