#include "PatternAssign.h"
#include "PatternIf.h"
#include "PatternSwitch.h"
#include "PatternWhile.h"
#include "QueryPreprocessorError.h"
#include "QueryPreprocessorHelper.h"
#include "QueryPreprocessorPatternParser.h"

constexpr char BRACKET_CLOSE = ')';
constexpr char BRACKET_OPEN = '(';
constexpr char DELIMETER = ',';
constexpr char EMPTY[] = "";
constexpr char QUOTE = '"';
constexpr char UNDERSCOR = '_';

// Regular Expressions
constexpr char EXPRESSION[] = "^[a-zA-z0-9()*/%+\\- ]+$";
const std::regex QueryPreprocessorPatternParser::EXPRESSION_REGEX(EXPRESSION);

// Initializes a newly created QueryPreprocessorPatternParser.
QueryPreprocessorPatternParser::QueryPreprocessorPatternParser(std::string& clause, ProcessedQuery& query) 
	: CLAUSE(clause), query(query) {}

// Parses the pattern clause.
// Returns true if parsing is successful and false if unsucessful.
void QueryPreprocessorPatternParser::parse() {
	size_t synonymSize = CLAUSE.find(BRACKET_OPEN);
	size_t closeBracketPos = CLAUSE.rfind(BRACKET_CLOSE);

	// open bracket must exist
	if (synonymSize == std::string::npos) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}

	// close bracket must be the last character in pattern clause
	if (CLAUSE[CLAUSE.size() - 1] == closeBracketPos) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}

	// extract synonym and its design entity
	// synonym must have already exist in declarations
	std::string synonym = CLAUSE.substr(0, synonymSize);
	if (!query.hasSynonym(synonym)) {
		throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
	}

	Type designEntity = query.getDesignEntity(synonym);
	DesignEntity target(synonym, designEntity);

	// extract parameters
	size_t paramStart = synonymSize + 1;
	size_t paramSize = closeBracketPos - synonymSize - 1;
	std::string params = CLAUSE.substr(paramStart, paramSize);

	// parameters should have at least one comma as delimeter
	size_t paramOneSize = params.find(DELIMETER);
	if (CLAUSE[CLAUSE.size() - 1] == closeBracketPos) {
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}

	// tokenise parameters
	std::string paramOneString = params.substr(0, paramOneSize);
	std::string paramTwoString = params.substr(paramOneSize + 1);

	// parse first parameter
	// first parameter must be of entity reference type
	DesignEntity paramOne = parseEntRef(paramOneString);

	if (designEntity == Type::ASSIGN) {
		// assign pattern
		DesignEntity paramTwo = parseExpression(paramTwoString);
		PatternAssign* pattern = new PatternAssign(target, paramOne, paramTwo);
		query.addClause(pattern, CLAUSE);
	}
	else if (designEntity == Type::WHILE && paramTwoString == "_") {
		// while pattern
		PatternWhile* pattern = new PatternWhile(target, paramOne);
		query.addClause(pattern, CLAUSE);
	}
	else if (designEntity == Type::IF && paramTwoString == "_,_") {
		// if pattern
		PatternIf* pattern = new PatternIf(target, paramOne);
		query.addClause(pattern, CLAUSE);
	}
	else if (designEntity == Type::SWITCH && paramTwoString == "_") {
		// switch
		PatternSwitch* pattern = new PatternSwitch(target, paramOne);
		query.addClause(pattern, CLAUSE);
	}
	else {
		// not a valid pattern clause
		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}
}

// Parse entRef parameters of pattern clauses.
// Allowable values are underscore, variable name, and variable synonym.
// Returns DesignEntity of Type::INVALID if parameter cannot be parsed.
DesignEntity QueryPreprocessorPatternParser::parseEntRef(const std::string& entRef) const {
	DesignEntity param = QueryPreprocessorHelper::getParam(entRef, query);
	std::vector<Type> legalTypes = { Type::UNDERSCORE, Type::VARIABLE };
	if (param.isAnyType(legalTypes) || param.isVar()) {
		return param;
	}

	throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
}

// Parse expression parameters of pattern clauses.
// Returns DesignEntity of Type::INVALID if parameter cannot be parsed.
DesignEntity QueryPreprocessorPatternParser::parseExpression(const std::string& expression) const {
	if (expression == "_") {
		// underscore
		return DesignEntity(EMPTY, Type::UNDERSCORE);
	}
	else if (expression.front() == UNDERSCOR 
		&& expression[1] == QUOTE
		&& expression.back() == UNDERSCOR
		&& expression[expression.size() - 2] == QUOTE) {
		// sub match
		std::string value = expression.substr(2, expression.size() - 4);
		value = QueryPreprocessorHelper::getPostFix(value);

		if (regex_match(value, EXPRESSION_REGEX)) {
			return DesignEntity(value, Type::PATTERN_SUB);
		}
	}
	else if (expression.front() == QUOTE && expression.back() == QUOTE) {
		// exact match
		std::string value = expression.substr(1, expression.size() - 2);
		value = QueryPreprocessorHelper::getPostFix(expression);

		if (regex_match(value, EXPRESSION_REGEX)) {
			return DesignEntity(value, Type::PATTERN_EXACT);
		}
	}

	throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
}
