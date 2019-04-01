#include "Affects.h"
#include "AffectsT.h"
#include "Clause.h"
#include "Follows.h"
#include "FollowsT.h"
#include "ModifiesS.h"
#include "NextT.h"
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
		withClausesSet.insert(withClause);
		clausesString.insert(clauseString);
	}
}

bool ProcessedQuery::hasSynonym(const std::string& synonym) {
	return declarations.find(synonym) != declarations.end();
}

Type ProcessedQuery::getDesignEntity(std::string& synonym) {
	return declarations.find(synonym)->second;
}

void ProcessedQuery::sortClauses() {
	for (auto& clause : clauses) {
		size_t noOfSynonyms = clause->getNumOfSynonyms();
		if (noOfSynonyms == 0) {
			// boolean clauses
			booleanClauses.push_back(clause);
			clauses.erase(clause);
		} else if (dynamic_cast<Affects*>(clause) != nullptr) {
			affectsClauses.push_back(clause);
			clauses.erase(clause);
		} else if (dynamic_cast<AffectsT*>(clause) != nullptr) {
			affectsTClauses.push_back(clause);
			clauses.erase(clause);
		} else if (dynamic_cast<NextT*>(clause) != nullptr) {
			nextTClauses.push_back(clause);
			clauses.erase(clause);
		} else {
			otherClauses.push_back(clause);
			clauses.erase(clause);
		}
	}

	for (auto& clause : withClausesSet) {
		size_t noOfSynonyms = clause->getNumOfSynonyms();
		if (noOfSynonyms == 0) {
			// boolean clauses
			booleanClauses.push_back(clause);
			clauses.erase(clause);
		}
		else {
			withClauses.push_back(clause);
			clauses.erase(clause);
		}
	}
}