#include "ResultCache.h"

using namespace std;

void ResultCache::resetCache() {
	SynSynSame.clear();
	SynSynDiff.clear();
	SynUnderscore.clear();
	UnderscoreSyn.clear();
	SynFixed.clear();
	FixedSyn.clear();
	twoSynCacheResult.clear();
	oneSynCacheResult.clear();
}

bool ResultCache::cacheExists(Clause* clause) {
	// clear previous cache result
	twoSynCacheResult.clear();
	oneSynCacheResult.clear();

	ClauseType clauseType = clause->getClauseType();
	DesignEntity paraOne = clause->getParaOne();
	DesignEntity paraTwo = clause->getParaTwo();
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();

	if (isStmtType(paraOneType) && isStmtType(paraTwoType)) {
		CacheType cacheType = convertToCacheType(paraOneType, paraTwoType);

		if (paraOne.getValue() == paraTwo.getValue()) { // SynSynSame
			if (clauseType == ClauseType::NEXT_T) {
				if (SynSynSame.count(cacheType) > 0) {
					twoSynCacheResult = SynSynSame.at(cacheType);
					return true;
				}
				else {
					return false;
				}
			}
			else { // Affects and Affects* -> treat assign, stmt, progline as assign type
				if (SynSynSame.count(CacheType::ASSIGN_ASSIGN) > 0) {
					twoSynCacheResult = SynSynSame.at(CacheType::ASSIGN_ASSIGN);
					return true;
				}
				else {
					return false;
				}
			}
		}
		else { // SynSynDiff
			if (clauseType == ClauseType::NEXT_T) {
				if (SynSynDiff.count(cacheType) > 0) {
					twoSynCacheResult = SynSynDiff.at(cacheType);
					return true;
				}
				else {
					return false;
				}
			}
			else { // Affects and Affects* -> treat assign, stmt, progline as assign type
				if (SynSynDiff.count(CacheType::ASSIGN_ASSIGN) > 0) {
					twoSynCacheResult = SynSynDiff.at(CacheType::ASSIGN_ASSIGN);
					return true;
				}
				else {
					return false;
				}
			}
		}
	}
	else if (isStmtType(paraOneType) && paraTwoType == Type::UNDERSCORE) { // SynUnderscore
		if (clauseType == ClauseType::NEXT_T) {
			if (SynUnderscore.count(paraOneType) > 0) {
				oneSynCacheResult = SynUnderscore.at(paraOneType);
				return true;
			}
			else {
				return false;
			}
		}
		else { // Affects and Affects* -> treat assign, stmt, progline as assign type
			if (SynUnderscore.count(Type::ASSIGN) > 0) {
				oneSynCacheResult = SynUnderscore.at(Type::ASSIGN);
				return true;
			}
			else {
				return false;
			}
		}
	}
	else if (paraOneType == Type::UNDERSCORE && isStmtType(paraTwoType)) { // UnderscoreSyn
		if (clauseType == ClauseType::NEXT_T) {
			if (UnderscoreSyn.count(paraTwoType) > 0) {
				oneSynCacheResult = UnderscoreSyn.at(paraTwoType);
				return true;
			}
			else {
				return false;
			}
		}
		else { // Affects and Affects* -> treat assign, stmt, progline as assign type
			if (UnderscoreSyn.count(Type::ASSIGN) > 0) {
				oneSynCacheResult = UnderscoreSyn.at(Type::ASSIGN);
				return true;
			}
			else {
				return false;
			}
		}
	}
	else if (isStmtType(paraOneType) && paraTwoType == Type::FIXED) { // SynFixed
		if (clauseType == ClauseType::NEXT_T) {
			if (SynFixed.count(paraOneType) > 0 && SynFixed[paraOneType].count(stoi(clause->getParaTwo().getValue())) > 0) {
				oneSynCacheResult = SynFixed.at(paraOneType).at(stoi(clause->getParaTwo().getValue()));
				return true;
			}
			else {
				return false;
			}
		}
		else { // Affects and Affects* -> treat assign, stmt, progline as assign type
			if (SynFixed.count(Type::ASSIGN) > 0 && SynFixed[Type::ASSIGN].count(stoi(clause->getParaTwo().getValue())) > 0) {
				oneSynCacheResult = SynFixed.at(Type::ASSIGN).at(stoi(clause->getParaTwo().getValue()));
				return true;
			}
			else {
				return false;
			}
		}
	}
	else if (paraOneType == Type::FIXED && isStmtType(paraTwoType)) { // FixedSyn
		if (clauseType == ClauseType::NEXT_T) {
			if (FixedSyn.count(paraTwoType) > 0 && FixedSyn[paraTwoType].count(stoi(clause->getParaOne().getValue())) > 0) {
				oneSynCacheResult = FixedSyn.at(paraTwoType).at(stoi(clause->getParaOne().getValue()));
				return true;
			}
			else {
				return false;
			}
		}
		else { // Affects and Affects* -> treat assign, stmt, progline as assign type
			if (FixedSyn.count(Type::ASSIGN) > 0 && FixedSyn[Type::ASSIGN].count(stoi(clause->getParaOne().getValue())) > 0) {
				oneSynCacheResult = FixedSyn.at(Type::ASSIGN).at(stoi(clause->getParaOne().getValue()));
				return true;
			}
			else {
				return false;
			}
		}
	}
}

unordered_map<int, unordered_set<int>> ResultCache::getTwoSynCacheResult() {
	return twoSynCacheResult;
}

unordered_set<int> ResultCache::getOneSynCacheResult() {
	return oneSynCacheResult;
}

void ResultCache::storeInCache(Clause* clause, unordered_map<int, unordered_set<int>>& result) {
	ClauseType clauseType = clause->getClauseType();
	DesignEntity paraOne = clause->getParaOne();
	DesignEntity paraTwo = clause->getParaTwo();
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();

	if (isStmtType(paraOneType) && isStmtType(paraTwoType)) {
		CacheType cacheType = convertToCacheType(paraOneType, paraTwoType);

		if (paraOne.getValue() == paraTwo.getValue()) { // SynSynSame
			if (clauseType == ClauseType::NEXT_T) {
				SynSynSame[cacheType] = result;
			}
			else { // Affects and Affects* -> treat assign, stmt, progline as assign type
				SynSynSame[CacheType::ASSIGN_ASSIGN] = result;
			}
		}
		else { // SynSynDiff
			if (clauseType == ClauseType::NEXT_T) {
				SynSynDiff[cacheType] = result;
			}
			else { // Affects and Affects* -> treat assign, stmt, progline as assign type
				SynSynDiff[CacheType::ASSIGN_ASSIGN] = result;
			}
		}
	}
}

void ResultCache::storeInCache(Clause* clause,  unordered_set<int>& result) {
	ClauseType clauseType = clause->getClauseType();
	Type paraOneType = clause->getParaOne().getType();
	Type paraTwoType = clause->getParaTwo().getType();

	if (isStmtType(paraOneType) && paraTwoType == Type::UNDERSCORE) { // SynUnderscore
		if (clauseType == ClauseType::NEXT_T) {
			SynUnderscore[paraOneType] = result;
		}
		else { // Affects and Affects* -> treat assign, stmt, progline as assign type
			SynUnderscore[Type::ASSIGN] = result;
		}
	}
	else if (paraOneType == Type::UNDERSCORE && isStmtType(paraTwoType)) { // UnderscoreSyn
		if (clauseType == ClauseType::NEXT_T) {
			UnderscoreSyn[paraTwoType] = result;
		}
		else { // Affects and Affects* -> treat assign, stmt, progline as assign type
			UnderscoreSyn[Type::ASSIGN] = result;
		}
	}
	else if (isStmtType(paraOneType) && paraTwoType == Type::FIXED) { // SynFixed
		if (clauseType == ClauseType::NEXT_T) {
			SynFixed[paraOneType][stoi(clause->getParaTwo().getValue())] = result;
		}
		else { // Affects and Affects* -> treat assign, stmt, progline as assign type
			SynFixed[Type::ASSIGN][stoi(clause->getParaTwo().getValue())] = result;
		}
	}
	else if (paraOneType == Type::FIXED && isStmtType(paraTwoType)) { // FixedSyn
		if (clauseType == ClauseType::NEXT_T) {
			FixedSyn[paraTwoType][stoi(clause->getParaOne().getValue())] = result;
		}
		else { // Affects and Affects* -> treat assign, stmt, progline as assign type
			FixedSyn[Type::ASSIGN][stoi(clause->getParaOne().getValue())] = result;
		}
	}
}

bool ResultCache::isStmtType(Type type) {
	return  type == Type::STATEMENT || type == Type::PROGLINE || type == Type::READ || type == Type::PRINT || type == Type::CALL ||
		type == Type::WHILE || type == Type::IF || type == Type::ASSIGN;
}


CacheType ResultCache::convertToCacheType(Type type1, Type type2) {
	CacheType cacheType;
	//STATEMENT, PROGLINE, READ, PRINT, CALL, WHILE, IF, ASSIGN
	switch (type1) {
		case Type::STATEMENT:
			cacheType = startWithStatement(type2);
			break;
		case Type::PROGLINE:
			cacheType = startWithProgline(type2);
			break;
		case Type::READ:
			cacheType = startWithRead(type2);
			break;
		case Type::PRINT:
			cacheType = startWithPrint(type2);
			break;
		case Type::CALL:
			cacheType = startWithCall(type2);
			break;
		case Type::WHILE:
			cacheType = startWithWhile(type2);
			break;
		case Type::IF:
			cacheType = startWithIf(type2);
			break;
		case Type::ASSIGN:
			cacheType = startWithAssign(type2);
			break;
	}
	return cacheType;
}

CacheType ResultCache::startWithStatement(Type type2) {
	CacheType cacheType;

	switch (type2) {
	case Type::STATEMENT:
		cacheType = CacheType::STMT_STMT;
		break;
	case Type::PROGLINE:
		cacheType = CacheType::STMT_PROGLINE;
		break;
	case Type::READ:
		cacheType = CacheType::STMT_READ;
		break;
	case Type::PRINT:
		cacheType = CacheType::STMT_PRINT;
		break;
	case Type::CALL:
		cacheType = CacheType::STMT_CALL;
		break;
	case Type::WHILE:
		cacheType = CacheType::STMT_WHILE;
		break;
	case Type::IF:
		cacheType = CacheType::STMT_IF;
		break;
	case Type::ASSIGN:
		cacheType = CacheType::STMT_ASSIGN;
		break;
	}
	return cacheType;
}

CacheType ResultCache::startWithProgline(Type type2) {
	CacheType cacheType;

	switch (type2) {
	case Type::STATEMENT:
		cacheType = CacheType::PROGLINE_STMT;
		break;
	case Type::PROGLINE:
		cacheType = CacheType::PROGLINE_PROGLINE;
		break;
	case Type::READ:
		cacheType = CacheType::PROGLINE_READ;
		break;
	case Type::PRINT:
		cacheType = CacheType::PROGLINE_PRINT;
		break;
	case Type::CALL:
		cacheType = CacheType::PROGLINE_CALL;
		break;
	case Type::WHILE:
		cacheType = CacheType::PROGLINE_WHILE;
		break;
	case Type::IF:
		cacheType = CacheType::PROGLINE_IF;
		break;
	case Type::ASSIGN:
		cacheType = CacheType::PROGLINE_ASSIGN;
		break;
	}
	return cacheType;
}

CacheType ResultCache::startWithRead(Type type2) {
	CacheType cacheType;

	switch (type2) {
	case Type::STATEMENT:
		cacheType = CacheType::READ_STMT;
		break;
	case Type::PROGLINE:
		cacheType = CacheType::READ_PROGLINE;
		break;
	case Type::READ:
		cacheType = CacheType::READ_READ;
		break;
	case Type::PRINT:
		cacheType = CacheType::READ_PRINT;
		break;
	case Type::CALL:
		cacheType = CacheType::READ_CALL;
		break;
	case Type::WHILE:
		cacheType = CacheType::READ_WHILE;
		break;
	case Type::IF:
		cacheType = CacheType::READ_IF;
		break;
	case Type::ASSIGN:
		cacheType = CacheType::READ_ASSIGN;
		break;
	}
	return cacheType;
}

CacheType ResultCache::startWithPrint(Type type2) {
	CacheType cacheType;

	switch (type2) {
	case Type::STATEMENT:
		cacheType = CacheType::PRINT_STMT;
		break;
	case Type::PROGLINE:
		cacheType = CacheType::PRINT_PROGLINE;
		break;
	case Type::READ:
		cacheType = CacheType::PRINT_READ;
		break;
	case Type::PRINT:
		cacheType = CacheType::PRINT_PRINT;
		break;
	case Type::CALL:
		cacheType = CacheType::PRINT_CALL;
		break;
	case Type::WHILE:
		cacheType = CacheType::PRINT_WHILE;
		break;
	case Type::IF:
		cacheType = CacheType::PRINT_IF;
		break;
	case Type::ASSIGN:
		cacheType = CacheType::PRINT_ASSIGN;
		break;
	}
	return cacheType;
}

CacheType ResultCache::startWithCall(Type type2) {
	CacheType cacheType;

	switch (type2) {
	case Type::STATEMENT:
		cacheType = CacheType::CALL_STMT;
		break;
	case Type::PROGLINE:
		cacheType = CacheType::CALL_PROGLINE;
		break;
	case Type::READ:
		cacheType = CacheType::CALL_READ;
		break;
	case Type::PRINT:
		cacheType = CacheType::CALL_PRINT;
		break;
	case Type::CALL:
		cacheType = CacheType::CALL_CALL;
		break;
	case Type::WHILE:
		cacheType = CacheType::CALL_WHILE;
		break;
	case Type::IF:
		cacheType = CacheType::CALL_IF;
		break;
	case Type::ASSIGN:
		cacheType = CacheType::CALL_ASSIGN;
		break;
	}
	return cacheType;
}

CacheType ResultCache::startWithWhile(Type type2) {
	CacheType cacheType;

	switch (type2) {
	case Type::STATEMENT:
		cacheType = CacheType::WHILE_STMT;
		break;
	case Type::PROGLINE:
		cacheType = CacheType::WHILE_PROGLINE;
		break;
	case Type::READ:
		cacheType = CacheType::WHILE_READ;
		break;
	case Type::PRINT:
		cacheType = CacheType::WHILE_PRINT;
		break;
	case Type::CALL:
		cacheType = CacheType::WHILE_CALL;
		break;
	case Type::WHILE:
		cacheType = CacheType::WHILE_WHILE;
		break;
	case Type::IF:
		cacheType = CacheType::WHILE_IF;
		break;
	case Type::ASSIGN:
		cacheType = CacheType::WHILE_ASSIGN;
		break;
	}
	return cacheType;
}

CacheType ResultCache::startWithIf(Type type2) {
	CacheType cacheType;

	switch (type2) {
	case Type::STATEMENT:
		cacheType = CacheType::IF_STMT;
		break;
	case Type::PROGLINE:
		cacheType = CacheType::IF_PROGLINE;
		break;
	case Type::READ:
		cacheType = CacheType::IF_READ;
		break;
	case Type::PRINT:
		cacheType = CacheType::IF_PRINT;
		break;
	case Type::CALL:
		cacheType = CacheType::IF_CALL;
		break;
	case Type::WHILE:
		cacheType = CacheType::IF_WHILE;
		break;
	case Type::IF:
		cacheType = CacheType::IF_IF;
		break;
	case Type::ASSIGN:
		cacheType = CacheType::IF_ASSIGN;
		break;
	}
	return cacheType;
}

CacheType ResultCache::startWithAssign(Type type2) {
	CacheType cacheType;

	switch (type2) {
	case Type::STATEMENT:
		cacheType = CacheType::ASSIGN_STMT;
		break;
	case Type::PROGLINE:
		cacheType = CacheType::ASSIGN_PROGLINE;
		break;
	case Type::READ:
		cacheType = CacheType::ASSIGN_READ;
		break;
	case Type::PRINT:
		cacheType = CacheType::ASSIGN_PRINT;
		break;
	case Type::CALL:
		cacheType = CacheType::ASSIGN_CALL;
		break;
	case Type::WHILE:
		cacheType = CacheType::ASSIGN_WHILE;
		break;
	case Type::IF:
		cacheType = CacheType::ASSIGN_IF;
		break;
	case Type::ASSIGN:
		cacheType = CacheType::ASSIGN_ASSIGN;
		break;
	}
	return cacheType;
}

// For testing purposes
void ResultCache::setSynSynSame(unordered_map<CacheType, unordered_map<int, unordered_set<int>>> result) {
	SynSynSame = result;
}

void ResultCache::setSynSynDiff(unordered_map<CacheType, unordered_map<int, unordered_set<int>>> result) {
	SynSynDiff = result;
}

void ResultCache::setSynUnderscore(unordered_map<Type, unordered_set<int>> result) {
	SynUnderscore = result;
}

void ResultCache::setUnderscoreSyn(unordered_map<Type, unordered_set<int>> result) {
	UnderscoreSyn = result;
}

void ResultCache::setSynFixed(unordered_map<Type, unordered_map<int, unordered_set<int>>> result) {
	SynFixed = result;
}

void ResultCache::setFixedSyn(unordered_map<Type, unordered_map<int, unordered_set<int>>> result) {
	FixedSyn = result;
}

unordered_map<CacheType, unordered_map<int, unordered_set<int>>> ResultCache::getSynSynSame() {
	return SynSynSame;
}

unordered_map<CacheType, unordered_map<int, unordered_set<int>>> ResultCache::getSynSynDiff() {
	return SynSynDiff;
}
unordered_map<Type, unordered_set<int>> ResultCache::getSynUnderscore() {
	return SynUnderscore;
}
unordered_map<Type, unordered_set<int>> ResultCache::getUnderscoreSyn() {
	return UnderscoreSyn;
}
unordered_map<Type, unordered_map<int, unordered_set<int>>> ResultCache::getSynFixed() {
	return SynFixed;
}
unordered_map<Type, unordered_map<int, unordered_set<int>>> ResultCache::getFixedSyn() {
	return FixedSyn;
}

bool ResultCache::isEquals(ResultCache other) {
	return SynSynSame == other.getSynSynSame() && SynSynDiff == other.getSynSynDiff() && SynUnderscore == other.getSynUnderscore() &&
		UnderscoreSyn == other.getUnderscoreSyn() && SynFixed == other.getSynFixed() && FixedSyn == other.getFixedSyn();
}
