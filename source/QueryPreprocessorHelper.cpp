#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "AttrRef.h"
#include "DesignEntity.h"
#include "ProcessedQuery.h"
#include "Type.h"
#include "QueryPreprocessorError.h"
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
	{"switch", Type::SWITCH},
	{"variable", Type::VARIABLE},
	{"while", Type::WHILE}
};

const std::unordered_map<Type, std::string> QueryPreprocessorHelper::TYPE_TO_STRING = {
	{Type::ASSIGN, "assign"},
	{Type::CALL, "call"},
	{Type::CONSTANT, "constant"},
	{Type::FIXED, "fixed"},
	{Type::IF, "if"},
	{Type::PRINT, "print"},
	{Type::PROCEDURE, "procedure"},
	{Type::PROGLINE, "prog_line"},
	{Type::READ, "read"},
	{Type::STATEMENT, "stmt"},
	{Type::SWITCH, "switch"},
	{Type::UNDERSCORE, "underscore"},
	{Type::VARIABLE, "variable"},
	{Type::WHILE, "while"},
	{Type::PATTERN_EXACT, "patternexact"},
	{Type::PATTERN_SUB, "patternsub"}
};

const std::unordered_map<std::string, AttrRef> QueryPreprocessorHelper::STRING_TO_ATTR_REF = {
	{"procName", AttrRef::PROC_NAME},
	{"varName", AttrRef::VAR_NAME},
	{"value", AttrRef::VALUE},
	{"stmt#", AttrRef::STMT_NUM}
};

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

std::string QueryPreprocessorHelper::getPostFix(const std::string& infix) {
	std::vector<std::string> assignTokens = std::vector<std::string>();
	std::string currToken = "";

	for (size_t index = 0; index < infix.length(); index++) {
		//assignment should have only alphanum and bracket/op. Less than 48 in ascii must be a bracket/op
		if (infix[index] < 48) {
			if (!currToken.empty()) {
				assignTokens.push_back(currToken);
				currToken = "";
			}
			std::string token = infix.substr(index, 1);
			assignTokens.push_back(token);
		}
		else {
			currToken += infix[index];
		}
	}

	if (!currToken.empty()) {
		assignTokens.push_back(currToken);
	}

	//form postfix expression using Dijkstra Shunting Yard
	std::vector<std::string> postfix = std::vector<std::string>();
	std::vector<std::string> opStack = std::vector<std::string>();
	for (size_t index = 0; index < assignTokens.size(); index++) {
		if (isInt(assignTokens[index]) || isVar(assignTokens[index])) {
			postfix.push_back(assignTokens[index]);
		}
		else if (assignTokens[index] == "(") {
			opStack.push_back(assignTokens[index]);
		}
		else if (assignTokens[index] == ")") {
			while (opStack.back() != "(") {
				postfix.push_back(opStack.back());
				opStack.pop_back();
			}
			opStack.pop_back();
		}
		else if (assignTokens[index] == "+" || assignTokens[index] == "-") {
			while (opStack.size() > 0 && opStack.back() != "(") {
				postfix.push_back(opStack.back());
				opStack.pop_back();
			}
			opStack.push_back(assignTokens[index]);
		}
		else if (assignTokens[index] == "*" || assignTokens[index] == "/" || assignTokens[index] == "%") {
			while (opStack.size() > 0 && (opStack.back() == "*" || opStack.back() == "/" || opStack.back() == "%")) {
				postfix.push_back(opStack.back());
				opStack.pop_back();
			}
			opStack.push_back(assignTokens[index]);
		}
	}

	while (opStack.size() > 0) {
		postfix.push_back(opStack.back());
		opStack.pop_back();
	}

	std::string postFix;

	for (std::string elem : postfix) {
		postFix += elem + ' ';
	}

	return postFix.substr(0, postFix.size() - 1);
}

bool QueryPreprocessorHelper::isInt(const std::string& target) {
	return target.find_first_not_of("0123456789") == std::string::npos;
}

bool QueryPreprocessorHelper::isVar(const std::string& target) {
	char firstChar = target.front();
	firstChar = tolower(firstChar);
	bool isValidFirstChar = firstChar > 96 && firstChar < 123;

	if (!isValidFirstChar) {
		return false;
	}

	for (size_t index = 1; index < target.size(); index++) {
		if (target[index] < 48
			|| (target[index] > 57 && target[index] < 65)
			|| (target[index] > 90 && target[index] < 96)
			|| target[index] > 122) {
			return false;
		}
	}

	return true;
}

DesignEntity QueryPreprocessorHelper::getParam(const std::string& param, ProcessedQuery& query) {
	size_t synonymSize = param.find('.');
	if (synonymSize == std::string::npos) {
		// param without attrRef
		if (param == "_") {
			// param is a underscore
			return DesignEntity("", Type::UNDERSCORE);
		}
		else if (isInt(param)) {
			// param is a statement number
			if (param == "0") {
				throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
			}
			return DesignEntity(param, Type::FIXED);
		}
		else if (param.front() == '"' && param.back() == '"') {
			// param is a variable

			// remove front and back quotes
			// validate that param is a variable
			std::string var = param.substr(1, param.size() - 2);
			if (isVar(var)) {
				return DesignEntity(var, Type::FIXED);
			}
			else {
				throw QueryPreprocessorError(ErrorType::SYNTACTIC);
			}
		}
		else if (query.hasSynonym(param)) {
			// param is a synonym
			return DesignEntity(param, query.getDesignEntity(param));
		}
		else if (isVar(param)) {
			throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
		}

		throw QueryPreprocessorError(ErrorType::SYNTACTIC);
	}
	else {
		// param with attrRef
		std::string synonym = param.substr(0, synonymSize);
		std::string attrRef = param.substr(synonymSize + 1);

		if (query.hasSynonym(synonym)) {
			// param is a synonym
			Type designEntity = query.getDesignEntity(synonym);

			if (designEntity == Type::CONSTANT && attrRef == "value") {
				return DesignEntity(synonym, Type::CONSTANT, AttrRef::VALUE);
			}
			else if (attrRef == "procName"
				&& (designEntity == Type::CALL
					|| designEntity == Type::PROCEDURE)) {
				// valid: call.procName, procedure.procName
				return DesignEntity(synonym, designEntity, AttrRef::PROC_NAME);
			}
			else if (attrRef == "varName"
				&& (designEntity == Type::PRINT
					|| designEntity == Type::READ
					|| designEntity == Type::VARIABLE)) {
				// valid: print.varName, read.varName, variable.varName
				return DesignEntity(synonym, designEntity, AttrRef::VAR_NAME);
			}
			else if (attrRef == "stmt#"
				&& (designEntity == Type::ASSIGN
					|| designEntity == Type::CALL
					|| designEntity == Type::IF
					|| designEntity == Type::PRINT
					|| designEntity == Type::READ
					|| designEntity == Type::STATEMENT
					|| designEntity == Type::SWITCH
					|| designEntity == Type::WHILE)) {
				return DesignEntity(synonym, designEntity, AttrRef::STMT_NUM);
			}

			throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
		}

		throw QueryPreprocessorError(query, ErrorType::SEMANTIC);
	}
}
