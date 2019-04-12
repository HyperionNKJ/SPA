#include <string>
#include "QueryPreprocessorError.h"
#include "QueryPreprocessorHelper.h"
#include "QueryPreprocessorWithParser.h"
#include "With.h"

constexpr char EQUAL = '=';
constexpr char EMPTY[] = "";

// Initializes a newly created QueryPreprocessorWithParser.
QueryPreprocessorWithParser::QueryPreprocessorWithParser(std::string& withCl, ProcessedQuery& query)
	: CLAUSE(withCl), query(query) {}

// Parses the with clause.
// Returns true if parsing is successful and false if unsucessful.
void QueryPreprocessorWithParser::parse() {
	// with clauses must have "="
	size_t lhsSize = CLAUSE.find(EQUAL);
	if (lhsSize == std::string::npos) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}

	// extract the lhs and rhs of with clause
	std::string lhs = CLAUSE.substr(0, lhsSize);
	std::string rhs = CLAUSE.substr(lhsSize + 1);

	if (rhs.find("=") != std::string::npos) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}

	DesignEntity paramOne = parseWithParam(lhs);
	DesignEntity paramTwo = parseWithParam(rhs);

	// with clause not added since it will always evaluate to true
	if (lhs == rhs) {
		return;
	}

	// with clause fixed statement number but different 
	if (paramOne.isType(Type::WITH_INTEGER) 
		&& paramTwo.isType(Type::WITH_INTEGER) 
		&& paramOne.getValue() != paramTwo.getValue()) {
		throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
	}

	// with clause fixed variable but different 
	if (paramOne.isType(Type::WITH_STRING)
		&& paramTwo.isType(Type::WITH_STRING)
		&& paramOne.getValue() != paramTwo.getValue()) {
		throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
	}

	// LHS AttrRef type must be the same as RHS AttrRef type
	isValidAttrRefComparator(paramOne, paramTwo);

	With* withClause = new With(paramOne, paramTwo);
	query.addWithClause(withClause, CLAUSE);
}

// Parses the individual parameters in with clause.
DesignEntity QueryPreprocessorWithParser::parseWithParam(const std::string& paramString) const {
	DesignEntity param = QueryPreprocessorHelper::getParam(paramString, query);

	if (param.isType(Type::UNDERSCORE)) {
		throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
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

	throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
}

// Validates that the comparation of two parameters is legal.
// Returns true if comparing the left and right hand side of the assignment is allowed.
void QueryPreprocessorWithParser::isValidAttrRefComparator(const DesignEntity& paramOne, const DesignEntity& paramTwo) const {
	bool isNameAttrRefOne = false;
	bool isNameAttrRefTwo = false;

	std::vector<AttrRef> legalAttrRefs = {AttrRef::PROC_NAME, AttrRef::VAR_NAME};

	if (paramOne.isAnyAttrRef(legalAttrRefs) || paramOne.isType(Type::WITH_STRING)) {
		isNameAttrRefOne = true;
	}

	if (paramTwo.isAnyAttrRef(legalAttrRefs) || paramTwo.isType(Type::WITH_STRING)) {
		isNameAttrRefTwo = true;
	}

	if (isNameAttrRefOne != isNameAttrRefTwo) {
		throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
	}
}
