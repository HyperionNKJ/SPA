#include <regex>
#include "DesignEntity.h"
#include "QueryPreprocessorHelper.h"
#include "QueryPreprocessorDeclareParser.h"

constexpr auto SPACE = ' ';
constexpr auto COMMA = ',';

// Initializes a newly created QueryPreprocessorDeclareParser.
QueryPreprocessorDeclareParser::QueryPreprocessorDeclareParser(const std::string& statement, 
		ProcessedQuery& query): STATEMENT(statement), query(query){

}

// Parse synonyms in declarative statement and add them to declarations.
bool QueryPreprocessorDeclareParser::parse() {
	size_t synonymIndex = STATEMENT.find_first_of(SPACE) + 1;

	// get design entity of this declaration statement
	std::string designEntityString = STATEMENT.substr(0, synonymIndex - 1);
	Type designEntity = QueryPreprocessorHelper::getType(designEntityString);

	// get synonyms of this declaration statement
	std::string synonymsString = STATEMENT.substr(synonymIndex);
	std::vector<std::string> synonyms = QueryPreprocessorHelper::split(synonymsString, COMMA);
	for (std::string& synonym : synonyms) {
		// check if synonym is a reserved word
		bool isReservedKeyword = QueryPreprocessorHelper::isReservedWord(synonym);
		if (isReservedKeyword) {
			return false;
		}

		// check if synonym already exist
		if (query.declarations.find(synonym) != query.declarations.end()) {
			return false;
		}

		// insert synonym into declarations map
		query.insertDeclaration(synonym, designEntity);
	}

	return true;
}
