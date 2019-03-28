#include <string>
#include "QueryPreprocessorWithParser.h"
#include "QueryPreprocessorHelper.h"
#include "With.h"

constexpr auto EQUAL = '=';

QueryPreprocessorWithParser::QueryPreprocessorWithParser(std::string& withCl, ProcessedQuery& query)
	: WITH_CL(withCl), query(query) {
}

bool QueryPreprocessorWithParser::parse() {
	size_t lhsSize = WITH_CL.find(EQUAL);
	if (lhsSize == std::string::npos) {
		return false;
	}

	std::string lhs = WITH_CL.substr(0, lhsSize);
	std::string rhs = WITH_CL.substr(lhsSize + 1);

	DesignEntity paramOne = QueryPreprocessorHelper::getParam(lhs, query);
	DesignEntity paramTwo = QueryPreprocessorHelper::getParam(rhs, query);

	Type designEntityOne = paramOne.getType();
	Type designEntityTwo = paramTwo.getType();

	if (designEntityOne == Type::INVALID || designEntityOne == Type::UNDERSCORE) {
		return false;
	}

	if (designEntityTwo == Type::INVALID || designEntityTwo == Type::UNDERSCORE) {
		return false;
	}

	if (designEntityOne == Type::FIXED) {
		std::string& val = paramOne.getValue();
		if (QueryPreprocessorHelper::isInt(val)) {
			paramOne.setType(Type::WITH_INTEGER);
		}
		else {
			paramOne.setType(Type::WITH_STRING);
		}
	}

	if (designEntityTwo == Type::FIXED) {
		std::string& val = paramTwo.getValue();
		if (QueryPreprocessorHelper::isInt(val)) {
			paramTwo.setType(Type::WITH_INTEGER);
		}
		else {
			paramTwo.setType(Type::WITH_STRING);
		}
	}

	if (paramOne.getAttrRef() == AttrRef::UNASSIGNED
		&& designEntityOne != Type::WITH_INTEGER
		&& designEntityOne != Type::WITH_STRING
		&& designEntityOne != Type::PROGLINE) {
		return false;
	}

	if (paramTwo.getAttrRef() == AttrRef::UNASSIGNED
		&& designEntityTwo != Type::WITH_INTEGER
		&& designEntityTwo != Type::WITH_STRING
		&& designEntityTwo != Type::PROGLINE) {
		return false;
	}

	With* withClause = new With(paramOne, paramTwo);
	query.addWithClause(withClause);

	return true;
}
