#include "QueryPreprocessorDeclareParser.h"
#include "QueryPreprocessorError.h"
#include "QueryPreprocessorHelper.h"
#include "Type.h"

constexpr auto SPACE = ' ';
constexpr auto DELIMITER = ',';

// Initializes a newly created QueryPreprocessorDeclareParser.
QueryPreprocessorDeclareParser::QueryPreprocessorDeclareParser(const std::string& statement, ProcessedQuery& query)
	: STATEMENT(statement), query(query) {}

// Parses synonyms in declarative statement and adds them to declarations.
// Returns true if all the synonyms in the declarative statement can be added into declarations.
// False if there exist a synonym that cannot be added into declarations.
void QueryPreprocessorDeclareParser::parse() {
	// extract design entity of this declaration statement
	size_t designEntitySize = STATEMENT.find_first_of(SPACE);
	std::string designEntityString = STATEMENT.substr(0, designEntitySize);
	Type designEntity = QueryPreprocessorHelper::getType(designEntityString);

	// extract the synonyms of this declaration statement
	std::string synonymsString = STATEMENT.substr(designEntitySize + 1);
	std::vector<std::string> synonyms = QueryPreprocessorHelper::split(synonymsString, DELIMITER);
	for (std::string& synonym : synonyms) {
		// insert synonym into declarations
		bool status = query.insertDeclaration(synonym, designEntity);
		if (!status) {
			throw QueryPreprocessorError(ErrorType::SYNTACTIC);
		}
	}
}
