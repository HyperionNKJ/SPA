#include "QueryPreprocessorDeclareParser.h"
#include "QueryPreprocessorError.h"
#include "QueryPreprocessorHelper.h"
#include "QueryPreprocessorPatternParser.h"
#include "QueryPreprocessorResultParser.h"
#include "QueryPreprocessorSelectParser.h"
#include "QueryPreprocessorSuchThatParser.h"
#include "QueryPreprocessorWithParser.h"

constexpr auto SPACE = ' ';

// Initializes a newly created QueryPreprocessorSelectParser.
QueryPreprocessorSelectParser::QueryPreprocessorSelectParser(const string& statement, ProcessedQuery& query)
	: STATEMENT(statement), query(query) {}

// Parse select clause.
void QueryPreprocessorSelectParser::parse() {
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
	parseResultCl.parse();

	if (selectCl.size() == 0) {
		// case 0: there is only a result clause
		return;
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
			throw QueryPreprocessorError(ErrorType::SYNTACTIC);
		}

		typeSize = type.size() + 1;

		std::string clause;

		size_t clauseEndPos = selectCl.find(SPACE, typeSize + 3);
		if (clauseEndPos == std::string::npos) {
			clause = selectCl.substr(typeSize);
			selectCl = "";
		}
		else {
			clause = selectCl.substr(typeSize, clauseEndPos - typeSize);
			selectCl.erase(0, clauseEndPos + 1);
		}

		if (type == "such that") {
			QueryPreprocessorSuchThatParser parseSuchThatCl(clause, query);
			parseSuchThatCl.parse();
		}
		else if (type == "pattern") {
			QueryPreprocessorPatternParser parsePatternCl(clause, query);
			parsePatternCl.parse();
		}
		else if (type == "with") {
			QueryPreprocessorWithParser parseWithCl(clause, query);
			parseWithCl.parse();
		}
		else {
			// Clauses should be in the form of rel(...)
			size_t relSize = clause.find('(');
			std::string rel = clause.substr(0, relSize);

			if (query.hasSynonym(rel) && clauseType == ClauseType::PATTERN) {
				// rel is a declared synonym
				// pattern clause
				QueryPreprocessorPatternParser parsePatternCl(clause, query);
				parsePatternCl.parse();
			}
			else if (clause.find('=') != std::string::npos && clauseType == ClauseType::WITH) {
				QueryPreprocessorWithParser parseWithCl(clause, query);
				parseWithCl.parse();
			}
			else if (clauseType == ClauseType::SUCH_THAT) {
				QueryPreprocessorSuchThatParser parseSuchThatCl(clause, query);
				parseSuchThatCl.parse();
			}
			else {
				throw QueryPreprocessorError(ErrorType::SYNTACTIC);;
			}
		}
	}
}
