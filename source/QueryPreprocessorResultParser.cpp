#include <regex>
#include "PatternAssign.h"
#include "PatternIf.h"
#include "PatternWhile.h"
#include "QueryPreprocessorHelper.h"
#include "QueryPreprocessorResultParser.h"

constexpr char CALL_OPERATOR = '.';
constexpr char COMMA = ',';
constexpr char TUPLE_BRACKET_L = '<';
constexpr char TUPLE_BRACKET_R = '>';
constexpr char EMPTY[] = "";

// Initializes a newly created QueryPreprocessorPatternParser.
QueryPreprocessorResultParser::QueryPreprocessorResultParser(const string& clause, ProcessedQuery& query)
	: CLAUSE(clause), query(query) {
}

// Parses the result clause.
// Returns true if parsing is successful and false if unsucessful.
bool QueryPreprocessorResultParser::parse() {
	// case 0: result clause is a boolean
	if (CLAUSE == "BOOLEAN") {
		DesignEntity element(EMPTY, Type::BOOLEAN);
		query.addResultClElement(element);

		return true;
	}

	// case 1: result clause is a tuple of element
	if (CLAUSE.front() == TUPLE_BRACKET_L && CLAUSE.back() == TUPLE_BRACKET_R) {
		// tokenize result clause into elements
		size_t clauseSize = CLAUSE.size() - 2;
		std::string elements = CLAUSE.substr(1, clauseSize);
		std::vector<std::string> elemList;
		elemList = QueryPreprocessorHelper::split(elements, COMMA);

		// parse individual element
		for (std::string elem : elemList) {
			bool status = addElement(elem);
			if (!status) {
				return false;
			}
		}

		return true;
	}

	// case 2: result clause is a element
	std::string elem = CLAUSE;
	bool status = addElement(elem);
	if (!status) {
		return false;
	}

	return true;
}

// Adds the element to the result clause list in ProcessedQuery
// Returns false if the element cannot be added
bool QueryPreprocessorResultParser::addElement(const std::string& elem) {
	DesignEntity element = QueryPreprocessorHelper::getParam(elem, query);

	if (!query.hasSynonym(element.getValue())) {
		return false;
	}

	std::vector<Type> validTypes = { 
		Type::ASSIGN, 
		Type::CALL,
		Type::CONSTANT, 
		Type::IF, 
		Type::PRINT, 
		Type::PROCEDURE, 
		Type::PROGLINE, 
		Type::READ, 
		Type::STATEMENT, 
		Type::VARIABLE, 
		Type::WHILE 
	};

	if (element.isAnyType(validTypes)) {
		query.addResultClElement(element);
		return true;
	}

	return false;
}
