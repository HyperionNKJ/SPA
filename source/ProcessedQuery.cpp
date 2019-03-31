#include "Clause.h"
#include "Follows.h"
#include "FollowsT.h"
#include "ModifiesS.h"
#include "Parent.h"
#include "ParentT.h"
#include "UsesS.h"
#include "ProcessedQuery.h"

ProcessedQuery::ProcessedQuery() {
}

bool ProcessedQuery::insertDeclaration(const std::string& synonym,
	const Type& type) {
	// same synonym cannot be declared twice
	if (hasSynonym(synonym)) {
		return false;
	}

	declarations.insert({ synonym, type });
	return true;
}

void ProcessedQuery::addResultClElement(const DesignEntity& element) {
	resultClElemList.push_back(element);
}

void ProcessedQuery::addClause(Clause* clause, const std::string& clauseString) {
	if (clausesString.find(clauseString) == clausesString.end()) {
		clauses.insert(clause);
		clausesString.insert(clauseString);
	}
}

void ProcessedQuery::addWithClause(Clause* withClause, const std::string& clauseString) {
	if (clausesString.find(clauseString) == clausesString.end()) {
		withClauses.insert(withClause);
		clausesString.insert(clauseString);
	}
}

bool ProcessedQuery::hasSynonym(const std::string& synonym) {
	return declarations.find(synonym) != declarations.end();
}

Type ProcessedQuery::getDesignEntity(std::string& synonym) {
	return declarations.find(synonym)->second;
}