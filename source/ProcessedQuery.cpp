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

struct ElementHasher {
	std::size_t operator()(const DesignEntity& element) const {
		return hash<string>()(element.toString());
	}
};

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
		withClauses.push_back(withClause);
		clausesString.insert(clauseString);
	}
}

bool ProcessedQuery::hasSynonym(const std::string& synonym) const {
	return declarations.find(synonym) != declarations.end();
}

Type ProcessedQuery::getDesignEntity(const std::string& synonym) const {
	return declarations.find(synonym)->second;
}

void ProcessedQuery::optimiseClauses() {
	std::unordered_map<DesignEntity, DesignEntity, ElementHasher> with;

	for (auto clause : withClauses) {
		if (clause->getNumOfSynonyms() == 1) {
			DesignEntity paramOne = clause->getParaOne();
			DesignEntity paramTwo = clause->getParaTwo();

			if (paramOne.isAnyType({ Type::WITH_INTEGER, Type::WITH_STRING })) {
				DesignEntity synonym (paramTwo.getValue(), paramTwo.getType());
				with.insert({ synonym , paramOne });
				with.insert({ paramTwo, paramOne });
			}
			else {
				DesignEntity synonym(paramOne.getValue(), paramOne.getType());
				with.insert({ synonym , paramTwo });
				with.insert({ paramOne, paramTwo });
			}
		}
	}

	bool isFirst = true;
	bool hasChanges = false;
	while (isFirst || hasChanges) {
		isFirst = false;
		hasChanges = false;

		for (auto clause : withClauses) {
			DesignEntity paramOne = clause->getParaOne();
			DesignEntity paramTwo = clause->getParaTwo();

			std::unordered_map<DesignEntity, DesignEntity>::const_iterator paramOneResult;
			std::unordered_map<DesignEntity, DesignEntity>::const_iterator paramTwoResult;

			paramOneResult = with.find(paramOne);
			paramTwoResult = with.find(paramTwo);

			if (paramOneResult != with.end() && paramTwoResult == with.end()) {
				DesignEntity synonym(paramTwo.getValue(), paramTwo.getType());
				with.insert({ synonym , paramOneResult->second });
				with.insert({ paramTwo, paramOneResult->second });
				hasChanges = true;
				continue;
			}

			if (paramOneResult == with.end() && paramTwoResult != with.end()) {
				DesignEntity synonym(paramOne.getValue(), paramOne.getType());
				with.insert({ synonym , paramTwoResult->second });
				with.insert({ paramOne, paramTwoResult->second });
				hasChanges = true;
				continue;
			}
		}
	}

	for (auto clause : nextTClauses) {
		DesignEntity paramOne = clause->getParaOne();
		DesignEntity paramTwo = clause->getParaTwo();

		std::unordered_map<DesignEntity, DesignEntity>::const_iterator paramOneResult;
		std::unordered_map<DesignEntity, DesignEntity>::const_iterator paramTwoResult;

		paramOneResult = with.find(paramOne);
		paramTwoResult = with.find(paramTwo);

		if (paramOneResult != with.end()) {
			DesignEntity replacement = paramOneResult->second;
			replacement.setType(Type::FIXED);
			clause->setParaOne(replacement);
		}

		if (paramTwoResult != with.end()) {
			DesignEntity replacement = paramTwoResult->second;
			replacement.setType(Type::FIXED);
			clause->setParaTwo(replacement);
		}
	}

	for (auto clause : affectsClauses) {
		DesignEntity paramOne = clause->getParaOne();
		DesignEntity paramTwo = clause->getParaTwo();

		std::unordered_map<DesignEntity, DesignEntity>::const_iterator paramOneResult;
		std::unordered_map<DesignEntity, DesignEntity>::const_iterator paramTwoResult;

		paramOneResult = with.find(paramOne);
		paramTwoResult = with.find(paramTwo);

		if (paramOneResult != with.end()) {
			DesignEntity replacement = paramOneResult->second;
			replacement.setType(Type::FIXED);
			clause->setParaOne(replacement);
		}

		if (paramTwoResult != with.end()) {
			DesignEntity replacement = paramOneResult->second;
			replacement.setType(Type::FIXED);
			clause->setParaTwo(replacement);
		}
	}

	for (auto clause : affectsTClauses) {
		DesignEntity paramOne = clause->getParaOne();
		DesignEntity paramTwo = clause->getParaTwo();

		std::unordered_map<DesignEntity, DesignEntity>::const_iterator paramOneResult;
		std::unordered_map<DesignEntity, DesignEntity>::const_iterator paramTwoResult;

		paramOneResult = with.find(paramOne);
		paramTwoResult = with.find(paramTwo);

		if (paramOneResult != with.end()) {
			DesignEntity replacement = paramOneResult->second;
			replacement.setType(Type::FIXED);
			clause->setParaOne(replacement);
		}

		if (paramTwoResult != with.end()) {
			DesignEntity replacement = paramOneResult->second;
			replacement.setType(Type::FIXED);
			clause->setParaTwo(replacement);
		}
	}

	for (auto clause : otherClauses) {
		DesignEntity paramOne = clause->getParaOne();
		DesignEntity paramTwo = clause->getParaTwo();

		std::unordered_map<DesignEntity, DesignEntity>::const_iterator paramOneResult;
		std::unordered_map<DesignEntity, DesignEntity>::const_iterator paramTwoResult;

		paramOneResult = with.find(paramOne);
		paramTwoResult = with.find(paramTwo);

		if (paramOneResult != with.end()) {
			DesignEntity replacement = paramOneResult->second;
			replacement.setType(Type::FIXED);
			clause->setParaOne(replacement);
		}

		if (paramTwoResult != with.end()) {
			DesignEntity replacement = paramTwoResult->second;
			replacement.setType(Type::FIXED);
			clause->setParaTwo(replacement);
		}
	}
}
