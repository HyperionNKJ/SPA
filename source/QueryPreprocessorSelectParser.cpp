#include "QueryPreprocessorDeclareParser.h"
#include "QueryPreprocessorHelper.h"
#include "QueryPreprocessorPatternParser.h"
#include "QueryPreprocessorResultParser.h"
#include "QueryPreprocessorSelectParser.h"
#include "QueryPreprocessorSuchThatParser.h"
#include "QueryPreprocessorWithParser.h"

constexpr auto SPACE = ' ';

// Initializes a newly created QueryPreprocessorSelectParser.
QueryPreprocessorSelectParser::QueryPreprocessorSelectParser(const string& statement, ProcessedQuery& query)
	: STATEMENT(statement), query(query) {
}

// Parse select clause.
bool QueryPreprocessorSelectParser::parse() {
	// remove "Select "
	std::string selectCl = STATEMENT;
	selectCl.erase(0, 7);

	// extract result clause
	size_t resultClSize = selectCl.find_first_of(SPACE);
	if (resultClSize == std::string::npos) {
		resultClSize = selectCl.size();
	}

	std::string resultCl = selectCl.substr(0, resultClSize);
	selectCl.erase(0, resultClSize + 1);
	
	QueryPreprocessorResultParser parseResultCl(resultCl, query);
	bool status = parseResultCl.parse();
	query = parseResultCl.query;

	if (selectCl.size() == 0 && status) {
		// case 0: there is only a result clause
		return true;
	}
	
	if (!status) {
		return false;
	}

	// case 1: there exist other clauses

	// extract such that, pattern, with clauses
	ClauseType clauseType = ClauseType::RESULT;
	while (selectCl.size() > 0) {
		std::string type;
		size_t typeSize;

		if (selectCl.find("such that ") == 0) {
			type = "such that";
			clauseType = ClauseType::SUCH_THAT;
		}
		else if (selectCl.find("pattern ") == 0) {
			type = "pattern";
			clauseType = ClauseType::PATTERN;
		}
		else if (selectCl.find("with ") == 0) {
			type = "with";
			clauseType = ClauseType::WITH;
		}
		else if (selectCl.find("and ") == 0) {
			type = "and";
		}
		else {
			return false;
		}

		typeSize = type.size() + 1;

		std::string clause;

		size_t clauseEndPos = selectCl.find(SPACE, typeSize + 6);
		if (clauseEndPos == std::string::npos) {
			clause = selectCl.substr(typeSize);
			selectCl = "";
		}
		else {
			clause = selectCl.substr(typeSize, clauseEndPos - typeSize);
			selectCl.erase(0, clauseEndPos + 1);
		}

		bool status;
		if (type == "such that") {
			QueryPreprocessorSuchThatParser parseSuchThatCl(clause, query);
			status = parseSuchThatCl.parse();
			query = parseSuchThatCl.query;
		}
		else if (type == "pattern") {
			QueryPreprocessorPatternParser parsePatternCl(clause, query);
			status = parsePatternCl.parse();
			query = parsePatternCl.query;
		}
		else if (type == "with") {
			QueryPreprocessorWithParser parseWithCl(clause, query);
			status = parseWithCl.parse();
			query = parseWithCl.query;
		}
		else {
			// Clauses should be in the form of rel(...)
			size_t relSize = clause.find('(');
			if (relSize == std::string::npos) {
				return false;
			}

			// If rel is found in declarations, the clause is a pattern clause
			std::string rel = clause.substr(0, relSize);
			if (query.declarations.find(rel) != query.declarations.end() && clauseType == ClauseType::PATTERN) {
				QueryPreprocessorPatternParser parsePatternCl(clause, query);
				status = parsePatternCl.parse();
				query = parsePatternCl.query;
			}
			else if (clause.find('=') != std::string::npos && clauseType == ClauseType::WITH) {
				QueryPreprocessorWithParser parseWithCl(clause, query);
				status = parseWithCl.parse();
				query = parseWithCl.query;
			}
			else if (clauseType == ClauseType::SUCH_THAT) {
				QueryPreprocessorSuchThatParser parseSuchThatCl(clause, query);
				status = parseSuchThatCl.parse();
				query = parseSuchThatCl.query;
			}
			else {
				return false;
			}
		}

		if (!status) {
			return false;
		}
	}

	return true;
}
