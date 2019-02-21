#pragma once

#include<unordered_map>
#include "StatementType.h"

class StatementTypeFactory {
public:
	// design entity type to string map and string map to design entity type
	static const std::unordered_map<StatementType, std::string> ST_TO_STRING;
	static const std::unordered_map<std::string, StatementType> STRING_TO_ST;

	// getters for retrieving string and enum of a statement type
	static const StatementType getStatementType(const std::string& statementTypeStr);
	static const std::string getStatementTypeString(const StatementType& statementTypeType);
};
