#pragma once

#include <unordered_map>
#include <unordered_set>
#include "Clause.h"

class ProcessedQuery {
private:
	std::unordered_map<std::string, Type> declarations;
	std::unordered_set<std::string> clausesString;
public:
	std::vector<DesignEntity> resultClElemList;

	std::vector<Clause*> affectsClauses;
	std::vector<Clause*> affectsTClauses;
	std::vector<Clause*> booleanClauses;
	std::vector<Clause*> nextTClauses;
	std::vector<Clause*> otherClauses;
	std::vector<Clause*> withClauses;

	ProcessedQuery();

	bool insertDeclaration(const std::string& synonym, const Type& designEntityType);

	void addResultClElement(const DesignEntity& element);
	void addClause(Clause* clause, const std::string& clauseString);
	void addWithClause(Clause* withClause, const std::string& clauseString);

	bool hasSynonym(const std::string& synonym) const;
	Type getDesignEntity(const std::string& synonym) const;
};
