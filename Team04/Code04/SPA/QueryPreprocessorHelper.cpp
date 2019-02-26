#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "QueryPreprocessorHelper.h"

const std::unordered_map<std::string, Type> QueryPreprocessorHelper::STRING_TO_TYPE = {
	{"assign", Type::ASSIGN},
	{"constant", Type::CONSTANT},
	{"if", Type::IF},
	{"print", Type::PRINT},
	{"procedure", Type::PROCEDURE},
	{"read", Type::READ},
	{"stmt", Type::STATEMENT},
	{"variable", Type::VARIABLE},
	{"while", Type::WHILE}
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
