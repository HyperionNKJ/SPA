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
	declarations.insert({ synonym, type });
	return true;
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

bool ProcessedQuery::hasSynonym(std::string& synonym) {
	return declarations.find(synonym) != declarations.end();
}

Type ProcessedQuery::getDesignEntity(std::string& synonym) {
	return declarations.find(synonym)->second;
}