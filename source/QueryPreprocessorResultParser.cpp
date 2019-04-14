#include "PatternAssign.h"
#include "PatternIf.h"
#include "PatternWhile.h"
#include "QueryPreprocessorError.h"
#include "QueryPreprocessorHelper.h"
#include "QueryPreprocessorResultParser.h"

constexpr char CALL_OPERATOR = '.';
constexpr char COMMA = ',';
constexpr char TUPLE_BRACKET_L = '<';
constexpr char TUPLE_BRACKET_R = '>';
constexpr char EMPTY[] = "";

const std::vector<Type> QueryPreprocessorResultParser::VALID_TYPES = {
		Type::ASSIGN,
		Type::CALL,
		Type::CONSTANT,
		Type::IF,
		Type::PRINT,
		Type::PROCEDURE,
		Type::PROGLINE,
		Type::READ,
		Type::STATEMENT,
		Type::SWITCH,
		Type::VARIABLE,
		Type::WHILE
};

// Initializes a newly created QueryPreprocessorPatternParser.
QueryPreprocessorResultParser::QueryPreprocessorResultParser(const string& clause, ProcessedQuery& query)
	: CLAUSE(clause), query(query) {}

// Parses the result clause.
// Returns true if parsing is successful and false if unsucessful.
void QueryPreprocessorResultParser::parse() {
	if (CLAUSE.back() == COMMA) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}

	// case 0: result clause is a boolean
	if (CLAUSE == "BOOLEAN" && !query.hasSynonym("BOOLEAN")) {
		DesignEntity element(EMPTY, Type::BOOLEAN);
		query.addResultClElement(element);

		return;
	}

	// case 1: result clause is a tuple of element
	if (CLAUSE.front() == TUPLE_BRACKET_L 
		&& CLAUSE.back() == TUPLE_BRACKET_R
		&& CLAUSE[CLAUSE.size() - 2] != COMMA) {
		// tokenize result clause into elements
		size_t clauseSize = CLAUSE.size() - 2;
		std::string elements = CLAUSE.substr(1, clauseSize);
		std::vector<std::string> elemList;
		elemList = QueryPreprocessorHelper::split(elements, COMMA);

		// parse individual element
		for (std::string elem : elemList) {
			addElement(elem);
		}

		return;
	}

	// case 2: result clause is a element
	std::string elem = CLAUSE;
	addElement(elem);
}

// Adds the element to the result clause list in ProcessedQuery
// Returns false if the element cannot be added
void QueryPreprocessorResultParser::addElement(const std::string& elem) {
	DesignEntity element = QueryPreprocessorHelper::getParam(elem, query);

	if (!query.hasSynonym(element.getValue()) || !element.isAnyType(VALID_TYPES)) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}

	query.addResultClElement(element);
}
