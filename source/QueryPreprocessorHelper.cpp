#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "AttrRef.h"
#include "Type.h"
#include "QueryPreprocessorHelper.h"

static std::string varNameRegex = "([[:alpha:]]([[:alnum:]])*)";
static std::string constantRegex = "[[:digit:]]+";
static std::string spaceRegex = "[[:s:]]*";
static std::string openCurlyRegex = "\\{";

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
	{Type::UNDERSCORE, "underscore"},
	{Type::VARIABLE, "variable"},
	{Type::WHILE, "while"}
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

bool isValidVarName(const std::string& line) {
	std::string varNameRegexString = spaceRegex + varNameRegex + spaceRegex;
	std::regex varRegex(varNameRegexString);
	if (!std::regex_match(line, varRegex)) {
		return false;
	}
	return true;
}

bool isValidConstant(const std::string& line) {
	std::string constantRegexString = spaceRegex + constantRegex + spaceRegex;
	std::regex constRegex(constantRegexString);
	if (!std::regex_match(line, constRegex)) {
		return false;
	}
	return true;
}

std::string QueryPreprocessorHelper::getPostFix(const std::string& infix) {
	//Separate variable names, constants and operation/brackets from each other
	std::vector<std::string> assignTokens = std::vector<std::string>();
	std::string lhsVar = infix.substr(0, infix.find_first_of("="));
	std::string rhs = infix.substr(infix.find_first_of("=") + 1, std::string::npos);
	std::string currToken = "";
	for (unsigned int i = 0; i < rhs.length(); i++) {
		//assignment should have only alphanum and bracket/op. Less than 48 in ascii must be a bracket/op
		if (rhs[i] < 48) {
			if (!currToken.empty()) {
				assignTokens.push_back(currToken);
				currToken = "";
			}
			assignTokens.push_back(rhs.substr(i, 1));
		}
		else {
			currToken += rhs[i];
		}
	}
	if (!currToken.empty()) {
		assignTokens.push_back(currToken);
	}
	//form postfix expression using Dijkstra Shunting Yard
	std::vector<std::string> postfixRHS = std::vector<std::string>();
	std::vector<std::string> opStack = std::vector<std::string>();
	for (unsigned int i = 0; i < assignTokens.size(); i++) {
		if (isValidConstant(assignTokens[i]) || isValidVarName(assignTokens[i])) {
			postfixRHS.push_back(assignTokens[i]);
		}
		else if (assignTokens[i] == "(") {
			opStack.push_back(assignTokens[i]);
		}
		else if (assignTokens[i] == ")") {
			while (opStack.back() != "(") {
				postfixRHS.push_back(opStack.back());
				opStack.pop_back();
			}
			opStack.pop_back();
		}
		else if (assignTokens[i] == "+" || assignTokens[i] == "-") {
			while (opStack.size() > 0 && (opStack.back() == "+" || opStack.back() == "-")) {
				postfixRHS.push_back(opStack.back());
				opStack.pop_back();
			}
			opStack.push_back(assignTokens[i]);
		}
		else if (assignTokens[i] == "*" || assignTokens[i] == "/" || assignTokens[i] == "%") {
			while (opStack.size() > 0 && (opStack.back() == "*" || opStack.back() == "/" || opStack.back() == "%")) {
				postfixRHS.push_back(opStack.back());
				opStack.pop_back();
			}
			opStack.push_back(assignTokens[i]);
		}
	}
	while (opStack.size() > 0) {
		postfixRHS.push_back(opStack.back());
		opStack.pop_back();
	}

	std::string postFix;

	for (std::string elem : postfixRHS) {
		postFix += elem + ' ';
	}

	return postFix.substr(0, postFix.size() - 1);
}