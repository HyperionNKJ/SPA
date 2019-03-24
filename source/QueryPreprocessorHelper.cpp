#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "AttrRef.h"
#include "Type.h"
#include "QueryPreprocessorHelper.h"

const std::unordered_map<std::string, Type> QueryPreprocessorHelper::STRING_TO_TYPE = {
	{"assign", Type::ASSIGN},
	{"call", Type::CALL},
	{"constant", Type::CONSTANT},
	{"if", Type::IF},
	{"print", Type::PRINT},
	{"procedure", Type::PROCEDURE},
	{"prog_line", Type::PROGLINE},
	{"read", Type::READ},
	{"stmt", Type::STATEMENT},
	{"variable", Type::VARIABLE},
	{"while", Type::WHILE}
};

const std::unordered_map<std::string, AttrRef> QueryPreprocessorHelper::STRING_TO_ATTR_REF = {
	{"procName", AttrRef::PROC_NAME},
	{"varName", AttrRef::VAR_NAME},
	{"value", AttrRef::VALUE},
	{"stmt#", AttrRef::STMT_NUM}
};

const std::unordered_set<std::string> QueryPreprocessorHelper::RESERVED_WORD = {
	"assign", "Affects", "call", "Calls", "constant", "Follows", "if", "Modifies",
	"Next", "Parent", "pattern", "print", "procedure", "read", "Select", "stmt",
	"Uses", "variable", "while", "with"
};

QueryPreprocessorHelper::QueryPreprocessorHelper() {

}

std::vector<std::string> QueryPreprocessorHelper::split(const std::string& target, const char& delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(target);
	std::string token;

	while (std::getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

Type QueryPreprocessorHelper::getType(const std::string& typeString) {
	return STRING_TO_TYPE.find(typeString)->second;
}

AttrRef QueryPreprocessorHelper::getAttrRef(const std::string& attrRefString) {
	return STRING_TO_ATTR_REF.find(attrRefString)->second;
}

bool QueryPreprocessorHelper::isReservedWord(const std::string& target) {
	return RESERVED_WORD.find(target) != RESERVED_WORD.end();
}
