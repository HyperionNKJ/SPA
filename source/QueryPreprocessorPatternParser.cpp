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

// Regular Expressions
constexpr char IDENT[] = "^[a-zA-z][a-zA-Z\\d]*$";
const regex QueryPreprocessorPatternParser::IDENT_REGEX(IDENT);

constexpr char INT[] = "^[123456789][\\d]*$";
const regex QueryPreprocessorPatternParser::INT_REGEX(INT);

constexpr char REL_REF[] = "^(Modifies|Uses|Calls|Calls\\*|Parent|Parent\\*|Follows|Follows\\*|Next|Next\\*|Affects|Affects\\*)$";
const regex QueryPreprocessorPatternParser::REL_REF_REGEX(REL_REF);

constexpr char EXPRESSION[] = "^[a-zA-z0-9()*\\/%+- ]+$";
const regex QueryPreprocessorPatternParser::EXPRESSION_REGEX(EXPRESSION);

// Initializes a newly created QueryPreprocessorPatternParser.
QueryPreprocessorPatternParser::QueryPreprocessorPatternParser(string& clause, 
	ProcessedQuery& query): clause(clause), query(query) {
	
}

// Parses the pattern clause. Returns true if parsing is successful and false 
// if unsucessful.
bool QueryPreprocessorPatternParser::parse() {
	size_t synonymSize = clause.find(BRACKET_OPEN);
	size_t closeBracketPos = clause.find(BRACKET_CLOSE);

	// open bracket must exist
	if (synonymSize == std::string::npos) {
		return false;
	}

	// first close bracket must be the last close bracket
	// close bracket must be the last character in pattern clause
	if (closeBracketPos + 1 != clause.size()) {
		return false;
	}

	// extract synonym
	// synonym must have already exist in declarations
	std::string synonymString = clause.substr(0, synonymSize);
	std::unordered_map<std::string, Type>::const_iterator element;
	element = query.declarations.find(synonymString);
	if (element == query.declarations.end()) {
		return false;
	}
	Type designEntity = element->second;
	DesignEntity synonym(synonymString, designEntity);

	// erase synonym and the brackets
	clause = clause.substr(synonymSize + 1, closeBracketPos - synonymSize - 1);

	// parse pattern parameters
	size_t paramOneSize = clause.find(DELIMETER);
	if (paramOneSize == std::string::npos) {
		return false;
	}

	std::string paramOneString = clause.substr(0, paramOneSize);
	std::string paramTwoString = clause.substr(paramOneSize + 1);

	if (designEntity == Type::ASSIGN) {
		// assign pattern
		DesignEntity paramOne = parseEntRef(paramOneString);
		DesignEntity paramTwo = parseExpression(paramTwoString);

		if (paramOne.getType() == Type::INVALID || paramTwo.getType() == Type::INVALID) {
			return false;
		}

		PatternAssign pattern(synonym, paramOne, paramTwo);
		query.addClause(&pattern);
	} else if (designEntity == Type::WHILE && paramTwoString == "_") {
		// while pattern
		DesignEntity paramOne = parseEntRef(paramOneString);

		if (paramOne.getType() == Type::INVALID) {
			return false;
		}

		PatternWhile pattern(synonym, paramOne);
		query.addClause(&pattern);
	} else if (designEntity == Type::IF && paramTwoString == "_,_") {
		// if pattern
		DesignEntity paramOne = parseEntRef(paramOneString);

		if (paramOne.getType() == Type::INVALID) {
			return false;
		}

		PatternIf pattern(synonym, paramOne);
		query.addClause(&pattern);
	} else {
		return false;
	}

	return true;
}

// Parse entRef parameters of pattern clauses.
// Returns DesignEntity of Type::INVALID if parameter cannot be parsed.
DesignEntity QueryPreprocessorPatternParser::parseEntRef(std::string& entRef) {
	if (entRef == "_") {
		// underscore
		return DesignEntity(EMPTY, Type::UNDERSCORE);
	} else if (entRef.front() == QUOTE && entRef.back() == QUOTE) {
		// constant
		entRef.erase(0);
		entRef.erase(entRef.size() - 1);

		if (!regex_match(entRef, IDENT_REGEX)) {
			return DesignEntity(EMPTY, Type::INVALID);
		}

		return DesignEntity(entRef, Type::FIXED);
	} else if (query.declarations.find(entRef) != query.declarations.end()) {
		// synonyms
		Type designEntity = query.declarations.find(entRef)->second;

		return DesignEntity(entRef, designEntity);
	}

	return DesignEntity(EMPTY, Type::INVALID);
}

// Parse expression parameters of pattern clauses.
// Returns DesignEntity of Type::INVALID if parameter cannot be parsed.
DesignEntity QueryPreprocessorPatternParser::parseExpression(std::string& expression) {
	if (expression.front() == '_'
			&& expression.back() == '_'
			&& expression[1] == QUOTE
			&& expression[expression.size() - 2] == QUOTE) {
		expression = expression.substr(2, expression.size() - 3);

		//convert to postfix
		expression = QueryPreprocessorHelper::getPostFix(expression);

		if (regex_match(expression, EXPRESSION_REGEX)) {
			return DesignEntity(expression, Type::INVALID);
		}

		return DesignEntity(expression, Type::PATTERN_SUB);
	} else if (expression.front() == QUOTE && expression.back() == QUOTE) {
		expression = expression.substr(1, expression.size() - 2);

		//convert to postfix
		expression = QueryPreprocessorHelper::getPostFix(expression);

		if (regex_match(expression, EXPRESSION_REGEX)) {
			return DesignEntity(expression, Type::INVALID);
		}

		return DesignEntity(expression, Type::PATTERN_EXACT);
	}
}
