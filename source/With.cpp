#include "With.h"

With::With(DesignEntity& refOne, DesignEntity& refTwo) {
	refOneIdentity = determineRefIdentity(refOne);
	refTwoIdentity = determineRefIdentity(refTwo);
	paraOne = refOne;
	paraTwo = refTwo;
	type = ClauseType::WITH;
	setSynonyms();
}

RefIdentity With::determineRefIdentity(DesignEntity& ref) {
	Type type = ref.getType();
	AttrRef attrRef = ref.getAttrRef();
	RefIdentity refIdentity;

	try {
		refIdentity = refIdentityTable.at(std::make_pair(type, attrRef));
	}
	catch (std::out_of_range e) { // case where ref is not allowed in with clause, for example const.varName (type = CONSTANT, attrRef = VAR_NAME)
		refIdentity = INVALID_REF;
	}
	return refIdentity;
}

Result With::evaluate(const PKB& pkb) {
	this->pkb = pkb;
	Result result;

	if (refOneIdentity == INVALID_REF || refTwoIdentity == INVALID_REF) {
		result.setPassed(false);
	}

	std::string refOneValue = paraOne.getValue();
	Type refOneType = paraOne.getType();
	AttrRef refOneAttRef = paraOne.getAttrRef();
	std::string refTwoValue = paraTwo.getValue();
	Type refTwoType = paraTwo.getType();
	AttrRef refTwoAttRef = paraTwo.getAttrRef();

	if (refOneIdentity == INTEGER) {
		if (refTwoIdentity == ATTR_REF_INT) {
			result = evaulateIntAttrRefInt(refOneValue, refTwoValue, refTwoType);
		}
		else if (refTwoIdentity == INTEGER) {
			result = evaulateIntInt(refOneValue, refTwoValue);
		}
		else if (refTwoIdentity == CONSTANT_VALUE) {
			result = evaulateIntConstVal(refOneValue, refTwoValue);
		}
		else if (refTwoIdentity == SYNONYM) {
			result = evaulateIntSyn(refOneValue, refTwoValue);
		}
		else {
			result.setPassed(false);
		}
	} 
	else if (refOneIdentity == ATTR_REF_INT) {
		if (refTwoIdentity == INTEGER) {
			result = evaulateIntAttrRefInt(refTwoValue, refOneValue, refOneType);
		}
		else if (refTwoIdentity == ATTR_REF_INT) {
			result = evaulateAttrRefIntAttrRefInt(refOneValue, refOneType, refTwoValue, refTwoType);
		}
		else if (refTwoIdentity == CONSTANT_VALUE) {
			result = evaulateAttrRefIntConstVal(refOneValue, refOneType, refTwoValue);
		}
		else if (refTwoIdentity == SYNONYM) {
			result = evaulateAttrRefIntSyn(refOneValue, refOneType, refTwoValue);
		}
		else {
			result.setPassed(false);
		}
	}
	else if (refOneIdentity == STRING) {
		if (refTwoIdentity == STRING) {
			result = evaluateStrStr(refOneValue, refTwoValue);
		}
		else if (refTwoIdentity == ATTR_REF_STRING) {
			result = evaluateStrAttrRefStr(refOneValue, refTwoValue, refTwoType);
		}
		else {
			result.setPassed(false);
		}
	}
	else if (refOneIdentity == ATTR_REF_STRING) {
		if (refTwoIdentity == STRING) {
			result = evaluateStrAttrRefStr(refTwoValue, refOneValue, refOneType);
		}
		else if (refTwoIdentity == ATTR_REF_STRING) {
			result = evaulateAttrRefStrAttrRefStr(refOneValue, refOneType, refTwoValue, refTwoType);
		}
		else {
			result.setPassed(false);
		}
	}
	else if (refOneIdentity == CONSTANT_VALUE) {
		if (refTwoIdentity == INTEGER) {
			result = evaulateIntConstVal(refTwoValue, refOneValue);
		}
		else if (refTwoIdentity == ATTR_REF_INT) {
			result = evaulateAttrRefIntConstVal(refTwoValue, refTwoType, refOneValue);
		}
		else if (refTwoIdentity == CONSTANT_VALUE) {
			result = evaulateConstValConstVal(refOneValue, refOneValue);
		}
		else if (refTwoIdentity == SYNONYM) {
			result = evaulateConstValSyn(refOneValue, refTwoValue);
		}
		else {
			result.setPassed(false);
		}
	}
	else if (refOneIdentity == SYNONYM) {
		if (refTwoIdentity == INTEGER) {
			result = evaulateIntSyn(refTwoValue, refOneValue);
		}
		else if (refTwoIdentity == ATTR_REF_INT) {
			result = evaulateAttrRefIntSyn(refTwoValue, refTwoType, refOneValue);
		}
		else if (refTwoIdentity == CONSTANT_VALUE) {
			result = evaulateConstValSyn(refTwoValue, refOneValue);
		}
		else if (refTwoIdentity == SYNONYM) {
			result = evaulateSynSyn(refOneValue, refTwoValue);
		}
		else {
			result.setPassed(false);
		}
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// e.g. 12 = 15
Result With::evaulateIntInt(const std::string& numOne, const std::string& numTwo) {
	return evaluateStrStr(numOne, numTwo);
} 

// e.g. 12 = n
Result With::evaulateIntSyn(const std::string& num, const std::string& progLineSyn) {
	Result result;
	unsigned int progLineNum = stoi(num);
	if (pkb.getAllStmts().size() >= progLineNum && progLineNum > 0 ) {
		result.setPassed(true);
		result.setAnswer(progLineSyn, progLineNum);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// e.g. 12 = c.value
Result With::evaulateIntConstVal(const std::string& num, const std::string& constSyn) {
	Result result;
	int constValue = stoi(num);
	std::unordered_set<int> constants = pkb.getAllConstant();
	if (constants.count(constValue)) {
		result.setPassed(true);
		result.setAnswer(constSyn, constValue);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// e.g. 12 = c.stmt#
Result With::evaulateIntAttrRefInt(const std::string& num, const std::string& synonym, const Type& type) {
	Result result;
	int stmtNum = stoi(num);
	std::unordered_set<int> validStatements = getValidStmts(type);
	if (validStatements.count(stmtNum)) {
		result.setPassed(true);
		result.setAnswer(synonym, stmtNum);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// e.g. "second" = "second"
Result With::evaluateStrStr(const std::string& stringOne, const std::string& stringTwo) {
	Result result;
	result.setPassed(stringOne == stringTwo);
	return result;
}

// e.g. "main" = p.procName
Result With::evaluateStrAttrRefStr(const std::string& name, const std::string& synonym, const Type& type) {
	Result result;
	std::unordered_set<std::string> validNames = getValidNames(type);
	if (validNames.count(name)) {
		result.setPassed(true);
		if (type == PROCEDURE || type == VARIABLE) {
			result.setAnswer(synonym, { name }, getIndexTable(type));
		}
		else { // type must be call or read or print
			result.setAnswer(synonym, getStmtNums(type, name));
		}
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// e.g. c.value = n
Result With::evaulateConstValSyn(const std::string& constSyn, const std::string& progLineSyn) {
	Result result;
	std::unordered_set<int> allConstants = pkb.getAllConstant();
	std::unordered_set<int> allProgLines = pkb.getAllStmts();
	std::unordered_map<int, int> intersectionMap = getCommonValueMap(allConstants, allProgLines);
	if (!intersectionMap.empty()) {
		result.setPassed(true);
		result.setAnswer(constSyn, progLineSyn, intersectionMap);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// e.g. c.value = c1.value
Result With::evaulateConstValConstVal(const std::string& constSynOne, const std::string& constSynTwo) {
	Result result;
	std::unordered_set<int> allConstants = pkb.getAllConstant();
	if (!allConstants.empty()) {
		result.setPassed(true);
		result.setAnswer(constSynOne, constSynTwo, getCommonValueMap(allConstants, allConstants));
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// e.g. n = n1
Result With::evaulateSynSyn(const std::string& progLineSynOne, const std::string& progLineSynTwo) {
	Result result;
	std::unordered_set<int> allProgLines = pkb.getAllStmts();
	result.setPassed(true); // program must contain at least one statement / program line. Hence n = n1 is always true!
	result.setAnswer(progLineSynOne, progLineSynTwo, getCommonValueMap(allProgLines, allProgLines));
	return result;
}

// e.g. w.stmt# = n
Result With::evaulateAttrRefIntSyn(const std::string& synonym, const Type& type, const std::string& progLineSyn) {
	Result result;
	std::unordered_set<int> validStatements = getValidStmts(type);
	if (!validStatements.empty()) {
		result.setPassed(true); // validStatements is subset of n. Hence, intersection is simply validStatements.
		result.setAnswer(synonym, progLineSyn, getCommonValueMap(validStatements, validStatements)); 
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// e.g. a.stmt# = c.value
Result With::evaulateAttrRefIntConstVal(const std::string& synonym, const Type& type, const std::string& constSyn) {
	Result result;
	std::unordered_set<int> validStatements = getValidStmts(type);
	std::unordered_set<int> allConstants = pkb.getAllConstant();
	std::unordered_map<int, int> intersectionMap = getCommonValueMap(allConstants, validStatements);
	if (!intersectionMap.empty()) {
		result.setPassed(true); 
		result.setAnswer(synonym, constSyn, intersectionMap);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// e.g. s.stmt# = w.stmt#
Result With::evaulateAttrRefIntAttrRefInt(const std::string& synOne, const Type& typeOne, const std::string& synTwo, const Type& typeTwo) {
	Result result;

	if (typeOne == STATEMENT || typeTwo == STATEMENT || typeOne == typeTwo) { 
		std::unordered_set<int> validStmtsOne = getValidStmts(typeOne);
		std::unordered_set<int> validStmtsTwo = getValidStmts(typeTwo);
		std::unordered_map<int, int> intersectionMap = getCommonValueMap(validStmtsOne, validStmtsTwo);
		if (!intersectionMap.empty()) {
			result.setPassed(true);
			result.setAnswer(synOne, synTwo, intersectionMap);
		}
		else {
			result.setPassed(false);
		}
	}
	else { // impossible for a statement to have 2 different types
		result.setPassed(false);
	}
	return result;
}

// e.g. v.varName = p.procName
Result With::evaulateAttrRefStrAttrRefStr(const std::string& synOne, const Type& typeOne, const std::string& synTwo, const Type& typeTwo) {
	Result result;
	std::unordered_set<std::string> validNamesOne = getValidNames(typeOne);
	std::unordered_set<std::string> validNamesTwo = getValidNames(typeTwo);
	std::unordered_set<std::string> commonString = getCommonString(validNamesOne, validNamesTwo);

	if (!commonString.empty()) {
		result.setPassed(true);
		bool oneIsStringTyped = (typeOne == PROCEDURE || typeOne == VARIABLE);
		bool twoIsStringTyped = (typeTwo == PROCEDURE || typeTwo == VARIABLE);

		if (oneIsStringTyped && twoIsStringTyped) { // e.g. proc.procName = var.varName, proc is string typed and var is string typed
			result.setAnswer(synOne, synTwo, getStrStrPairWithCommonStr(commonString), getIndexTable(typeOne), getIndexTable(typeTwo));
		}
		else if (!oneIsStringTyped && !twoIsStringTyped) { // e.g. call.procName = read.varName, call is int typed and read is int typed
			result.setAnswer(synOne, synTwo, getIntIntPairWithCommonStr(commonString, typeOne, typeTwo));
		}
		else if (oneIsStringTyped && !twoIsStringTyped) { // // e.g. var.varName = print.varName, var is string typed and print is int typed
			result.setAnswer(synOne, synTwo, getStrIntPairWithCommonStr(commonString, typeTwo), getIndexTable(typeOne));
		}
		else { // // e.g. read.varName = proc.procName, read is int typed and proc is string typed
			result.setAnswer(synTwo, synOne, getStrIntPairWithCommonStr(commonString, typeOne), getIndexTable(typeTwo));
		}
	}
	else {
		result.setPassed(false);
	}
	return result;
}

std::unordered_map<std::string, std::unordered_set<int>> With::getStrIntPairWithCommonStr(const std::unordered_set<std::string>& commonStr, const Type& type) {
	std::unordered_map<std::string, std::unordered_set<int>> answer;
	for (auto str : commonStr) {
		std::unordered_set<int> validStmts = getStmtNums(type, str);
		answer.insert({ str, validStmts });
	}
	return answer;
}

std::unordered_map<int, std::unordered_set<int>> With::getIntIntPairWithCommonStr(const std::unordered_set<std::string>& commonStr, const Type& typeOne, const Type& typeTwo) {
	std::unordered_map<int, std::unordered_set<int>> stmtPair;
	for (auto str : commonStr) {
		std::unordered_set<int> validStmtsOne = getStmtNums(typeOne, str);
		std::unordered_set<int> validStmtsTwo = getStmtNums(typeTwo, str);
		for (auto stmtNum : validStmtsOne) {
			stmtPair.insert({ stmtNum, validStmtsTwo });
		}
	}
	return stmtPair;
}

// Function to duplicate a string set to map = {x, {x}}. This format is required by Result's setAnswer function.
std::unordered_map<std::string, std::unordered_set<std::string>> With::getStrStrPairWithCommonStr(const std::unordered_set<std::string>& set) {
	std::unordered_map<std::string, std::unordered_set<std::string>> duplicatedMap;
	for (auto str : set) {
		duplicatedMap.insert({ str, {str} });
	}
	return duplicatedMap;
}

std::unordered_set<int> With::getValidStmts(const Type& type) {
	switch (type) {
	case STATEMENT:
		return pkb.getAllStmts();
	case READ:
		return pkb.getReadStmts();
	case PRINT:
		return pkb.getPrintStmts();
	case CALL:
		return pkb.getCallStmts();
	case WHILE:
		return pkb.getWhileStmts();
	case IF:
		return pkb.getIfStmts();
	case SWITCH:
		return pkb.getSwitchStmts(); // extension
	case ASSIGN:
		return pkb.getAssignStmts();
	default:
		return {};
	}
}

std::unordered_set<std::string> With::getValidNames(const Type& type) {
	switch (type) {
	case PROCEDURE:
		return pkb.getAllProcedures();
	case CALL:
		return pkb.getCallProcNames();
	case VARIABLE:
		return pkb.getAllVariables();
	case READ:
		return pkb.getReadVarNames();
	case PRINT:
		return pkb.getPrintVarNames();
	default:
		return {};
	}
}

std::unordered_map<std::string, int> With::getIndexTable(const Type& type) {
	switch (type) {
	case PROCEDURE:
		return pkb.getProcTable();
	case VARIABLE:
		return pkb.getVarTable();
	default:
		return {};
	}
}

std::unordered_set<int> With::getStmtNums(const Type& type, const std::string& name) {
	switch (type) {
	case CALL:
		return pkb.getCallStmtsWithProc(name);
	case READ:
		return pkb.getReadStmtsWithVar(name);
	case PRINT:
		return pkb.getPrintStmtsWithVar(name);
	default:
		return {};
	}
}

// Utility function to find intersection of 2 int sets. Can also be used to duplicate a set to map = {x, x} e.g. Set = {1, 2, 3} Map = { {1, 1}, {2, 2}, {3, 3} }
std::unordered_map<int, int> With::getCommonValueMap(const std::unordered_set<int>& setOne, const std::unordered_set<int>& setTwo) {
	std::unordered_map<int, int> intersectionMap;
	for (auto elem : setOne) {
		if (setTwo.count(elem)) {
			intersectionMap.insert({ elem, elem });
		}
	}
	return intersectionMap;
}

// Utility function to find intersection of 2 string sets.
std::unordered_set<std::string> With::getCommonString(const std::unordered_set<std::string>& setOne, const std::unordered_set<std::string>& setTwo) {
	std::unordered_set<std::string> intersectionMap;
	for (auto elem : setOne) {
		if (setTwo.count(elem)) {
			intersectionMap.insert(elem);
		}
	}
	return intersectionMap;
}
