#pragma once

#include <regex>
#include <unordered_set>
#include "ProcessedQuery.h"
#include "Type.h"
#include "Statement.h"

class QueryParser {
public:
	QueryParser(const std::vector<Statement>& statements);
	bool parse();
	ProcessedQuery getQuery();
private:
	static const std::string REL;
	static const std::string IDENT;
	static const std::string INT;
	static const std::string STMT;
	static const std::string ENT;
	static const std::string ENT_STMT;

	static const std::unordered_set<std::string> KEYWORDS;

	static const std::regex SELECT_STMT_REGEX;
	static const std::regex SUCH_THAT_CLAUSE_REGEX;
	static const std::regex STMT_REGEX;
	static const std::regex ENT_REGEX;
	static const std::regex PATTERN_CLAUSE_REGEX;
	static const std::regex EXPRESSION_REGEX;

	std::vector<Statement> STATEMENTS;
	ProcessedQuery query;

	bool parseDeclarationStatement(const Statement& statement);
	bool parseSelectStatement(const Statement& statement);
	bool parseSuchThatClause(const std::string& clause);
	bool parsePatternClause(const std::string& clause);

	static Type getParameterType(const std::string value);
	static RelationshipType getRelationshipType(const std::string& type);
};