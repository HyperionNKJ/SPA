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
	return declarations.insert({ synonym, type }).second;
}

void ProcessedQuery::addResultClElement(const DesignEntity& element) {
	resultClElemList.push_back(element);
}

void ProcessedQuery::addClause(Clause* clause) {
	clauses.insert(clause);
}

void ProcessedQuery::addWithClause(Clause* withClause) {
	withClauses.insert(withClause);
}