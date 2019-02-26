#include <string>
#include <unordered_map>
#include "StatementTypeFactory.h"

const std::unordered_map<StatementType, std::string> StatementTypeFactory::ST_TO_STRING{
	{StatementType::DECLARATION, "declaration"},
	{StatementType::SELECT,      "Select"},
	{StatementType::MODIFIES,    "Modifies"},
	{StatementType::USES,        "Uses"},
	{StatementType::PARENT,      "Parent"},
	{StatementType::PARENT_T,    "Parent*"},
	{StatementType::FOLLOWS,     "Follows"},
	{StatementType::FOLLOWS_T,   "Follows*"},
	{StatementType::PATTERN,     "pattern"},
};

const std::unordered_map<std::string, StatementType> StatementTypeFactory::STRING_TO_ST{
	{"declaration", StatementType::DECLARATION},
	{"Select",      StatementType::SELECT},
	{"Modifies",    StatementType::MODIFIES},
	{"Uses",        StatementType::USES},
	{"Parent",      StatementType::PARENT},
	{"Parent*",     StatementType::PARENT_T},
	{"Follows",     StatementType::FOLLOWS},
	{"Follows*",    StatementType::FOLLOWS_T},
	{"pattern",     StatementType::PATTERN},
};

const StatementType StatementTypeFactory::getStatementType(const std::string& statementTypeString) {
	return STRING_TO_ST.find(statementTypeString)->second;
}

const std::string StatementTypeFactory::getStatementTypeString(const StatementType& statementType) {
	return ST_TO_STRING.find(statementType)->second;
}
