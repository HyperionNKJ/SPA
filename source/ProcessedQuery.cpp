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
		size_t noOfSynonyms = clause->getNumOfSynonyms();
		if (noOfSynonyms == 0) {
			// boolean clauses
			booleanClauses.push_back(clause);
		}
		else if (clause->getClauseType() == ClauseType::AFFECTS) {
			// affects clauses
			affectsClauses.push_back(clause);
		}
		else if (clause->getClauseType() == ClauseType::AFFECTS_T) {
			// affects* clauses
			affectsTClauses.push_back(clause);
		}
		else if (clause->getClauseType() == ClauseType::NEXT_T) {
			// next* clauses
			nextTClauses.push_back(clause);
		}
		else {
			// other clauses
			otherClauses.push_back(clause);
		}

		clausesString.insert(clauseString);
	}
}

void ProcessedQuery::addWithClause(Clause* withClause, const std::string& clauseString) {
	if (clausesString.find(clauseString) == clausesString.end()) {
		size_t noOfSynonyms = withClause->getNumOfSynonyms();
		if (noOfSynonyms == 0) {
			// boolean clauses
			booleanClauses.push_back(withClause);
		}
		else {
			// with clauses
			withClauses.push_back(withClause);
		}

		clausesString.insert(clauseString);
	}
}

bool ProcessedQuery::hasSynonym(const std::string& synonym) const {
	return declarations.find(synonym) != declarations.end();
}

Type ProcessedQuery::getDesignEntity(const std::string& synonym) const {
	return declarations.find(synonym)->second;
}