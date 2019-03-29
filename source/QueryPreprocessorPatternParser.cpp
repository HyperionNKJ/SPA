#include <regex>
#include "PatternAssign.h"
#include "PatternIf.h"
#include "PatternWhile.h"
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
const regex QueryPreprocessorPatternParser::EXPRESSION_REGEX(EXPRESSION);

// Initializes a newly created QueryPreprocessorPatternParser.
QueryPreprocessorPatternParser::QueryPreprocessorPatternParser(string& clause,
	ProcessedQuery& query) : CLAUSE(clause), query(query) {
}

// Parses the pattern clause.
// Returns true if parsing is successful and false if unsucessful.
bool QueryPreprocessorPatternParser::parse() {
	size_t synonymSize = CLAUSE.find(BRACKET_OPEN);
	size_t closeBracketPos = CLAUSE.find(BRACKET_CLOSE);

	// open bracket must exist
	if (synonymSize == std::string::npos) {
		return false;
	}

	// first close bracket must be the last close bracket
	// close bracket must be the last character in pattern clause
	if (closeBracketPos + 1 != CLAUSE.size()) {
		return false;
	}

	// extract synonym and its design entity
	// synonym must have already exist in declarations
	std::string synonym = CLAUSE.substr(0, synonymSize);
	if (!query.hasSynonym(synonym)) {
		return false;
	}

	Type designEntity = query.getDesignEntity(synonym);
	DesignEntity target(synonym, designEntity);

	// remove synonym and the brackets
	// tokenise parameters
	// parse pattern parameters
	std::string params = CLAUSE.substr(synonymSize + 1, closeBracketPos - synonymSize - 1);
	size_t paramOneSize = params.find(DELIMETER);
	if (paramOneSize == std::string::npos) {
		return false;
	}

	std::string paramOneString = params.substr(0, paramOneSize);
	std::string paramTwoString = params.substr(paramOneSize + 1);

	if (designEntity == Type::ASSIGN) {
		// assign pattern
		DesignEntity paramOne = parseEntRef(paramOneString);
		DesignEntity paramTwo = parseExpression(paramTwoString);

		if (paramOne.getType() == Type::INVALID || paramTwo.getType() == Type::INVALID) {
			return false;
		}

		if (query.clausesString.find(CLAUSE) == query.clausesString.end()) {
			PatternAssign* pattern = new PatternAssign(target, paramOne, paramTwo);
			query.addClause(pattern);
			query.clausesString.insert(CLAUSE);
		}
	}
	else if (designEntity == Type::WHILE && paramTwoString == "_") {
		// while pattern
		DesignEntity paramOne = parseEntRef(paramOneString);

		if (paramOne.getType() == Type::INVALID) {
			return false;
		}

		if (query.clausesString.find(CLAUSE) == query.clausesString.end()) {
			PatternWhile* pattern = new PatternWhile(target, paramOne);
			query.addClause(pattern);
			query.clausesString.insert(CLAUSE);
		}
	}
	else if (designEntity == Type::IF && paramTwoString == "_,_") {
		// if pattern
		DesignEntity paramOne = parseEntRef(paramOneString);

		if (paramOne.getType() == Type::INVALID) {
			return false;
		}

		if (query.clausesString.find(CLAUSE) == query.clausesString.end()) {
			PatternIf* pattern = new PatternIf(target, paramOne);
			query.addClause(pattern);
			query.clausesString.insert(CLAUSE);
		}
	}
	else {
		return false;
	}

	return true;
}

// Parse entRef parameters of pattern clauses.
// Allowable values are underscore, variable name, and variable synonym.
// Returns DesignEntity of Type::INVALID if parameter cannot be parsed.
DesignEntity QueryPreprocessorPatternParser::parseEntRef(std::string& entRef) {
	DesignEntity param = QueryPreprocessorHelper::getParam(entRef, query);

	Type designEntityOne = param.getType();
	if (designEntityOne != Type::UNDERSCORE
		&& designEntityOne != VARIABLE
		&& !(designEntityOne == Type::FIXED
			&& QueryPreprocessorHelper::isVar(entRef))) {
		return DesignEntity(EMPTY, Type::INVALID);
	}

	return param;
}

// Parse expression parameters of pattern clauses.
// Returns DesignEntity of Type::INVALID if parameter cannot be parsed.
DesignEntity QueryPreprocessorPatternParser::parseExpression(std::string& expression) {
	if (expression == "_") {
		// underscore
		return DesignEntity(EMPTY, Type::UNDERSCORE);
	}
	else if (expression.front() == UNDERSCOR
		&& expression.back() == UNDERSCOR
		&& expression[1] == QUOTE
		&& expression[expression.size() - 2] == QUOTE) {
		// sub match
		expression = expression.substr(2, expression.size() - 4);
		expression = QueryPreprocessorHelper::getPostFix(expression);

		if (regex_match(expression, EXPRESSION_REGEX)) {
			return DesignEntity(expression, Type::PATTERN_SUB);
		}
	}
	else if (expression.front() == QUOTE && expression.back() == QUOTE) {
		// exact match
		expression = expression.substr(1, expression.size() - 2);
		expression = QueryPreprocessorHelper::getPostFix(expression);

		if (regex_match(expression, EXPRESSION_REGEX)) {
			return DesignEntity(expression, Type::PATTERN_EXACT);
		}
	}

	return DesignEntity(expression, Type::INVALID);
}
