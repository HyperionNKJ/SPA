#pragma once

#include <unordered_map>
#include <unordered_set>
#include "Clause.h"
#include "PatternClause.h"

class ProcessedQuery {
public:
	std::unordered_map<std::string, Type> declarations;
	std::vector<DesignEntity> resultClElemList;

	std::vector<Clause*> affectsClauses;
	std::vector<Clause*> affectsTClauses;
	std::vector<Clause*> booleanClauses;
	std::vector<Clause*> nextTClauses;
	std::vector<Clause*> otherClauses;
	std::vector<Clause*> withCLauses;

	std::unordered_set<Clause*> clauses;
	std::unordered_set<Clause*> withClauses;

	std::unordered_set<std::string> clausesString;
	std::unordered_set<std::string> withClausesString;

	ProcessedQuery();

	bool insertDeclaration(const std::string& synonym,
		const Type& designEntityType);

	void addResultClElement(const DesignEntity& element);
	void addClause(Clause* clause, const std::string& clauseString);
	void addWithClause(Clause* withClause);

	bool hasSynonym(const std::string& synonym);
	Type getDesignEntity(std::string& synonym);
};
