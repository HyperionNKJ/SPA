#include "Follows.h"

Follows::Follows(DesignEntity paraOne, DesignEntity paraTwo) : Clause(paraOne, paraTwo) {}

Result Follows::evaluate(PKB pkb) {
	this->pkb = pkb;
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	string paraOneValue = paraOne.getValue();
	string paraTwoValue = paraTwo.getValue();

	Result* result;

	if (paraOneType == FIXED) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT
			|| paraTwoType == WHILE || paraTwoType == IF || paraTwoType == ASSIGN) {
			result = this->evaluateFixedSynonym(paraOneValue, paraTwoValue, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateFixedUnderscore(paraOneValue);
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateFixedFixed(paraOneValue, paraTwoValue);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	}
	else if (paraOneType == STATEMENT || paraOneType == READ || paraOneType == PRINT 
		|| paraOneType == WHILE || paraOneType == IF || paraOneType == ASSIGN) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT
			|| paraTwoType == WHILE || paraTwoType == IF || paraTwoType == ASSIGN) {
			result = this->evaluateSynonymSynonym(paraOneValue, paraTwoValue, paraOneType, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateSynonymUnderscore(paraOneValue, paraOneType);
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateSynonymFixed(paraOneValue, paraTwoValue, paraOneType);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	}
	else if (paraOneType == UNDERSCORE) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT
			|| paraTwoType == WHILE || paraTwoType == IF || paraTwoType == ASSIGN) {
			result = this->evaluateUnderscoreSynonym(paraTwoValue, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateUnderscoreUnderscore();
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateUnderscoreFixed(paraTwoValue);
		}
		else {
			result = new Result();
			result->setPassed(false);
		}
	}
	else {
		result = new Result();
		result->setPassed(false);
	}
	return *result;
}

// case Follows(12, w)
Result* Follows::evaluateFixedSynonym(string leaderStmtNum, string followerSynonym, Type followerType) {
	Result* result = new Result();
	int answer = pkb.getFollowerOf(stoi(leaderStmtNum), followerType);
	if (answer != -1) {
		result->setPassed(true);
		result->setAnswer(followerSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Follows(3, _)
Result* Follows::evaluateFixedUnderscore(string leaderStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.hasFollower(stoi(leaderStmtNum)));
	return result;
}

// case Follows(4, 6)
Result* Follows::evaluateFixedFixed(string leaderStmtNum, string followerStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.isFollows(stoi(leaderStmtNum), stoi(followerStmtNum)));
	return result;
}

// case Follows(r, a)
Result* Follows::evaluateSynonymSynonym(string leaderSynonym, string followerSynonym, Type leaderType, Type followerType) {
	Result* result = new Result();
	unordered_map<int, int> answer = pkb.getLeaderFollowerPairs(leaderType, followerType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(leaderSynonym, followerSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Follows(pr, _)
Result* Follows::evaluateSynonymUnderscore(string leaderSynonym, Type leaderType) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getLeaderStmts(leaderType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(leaderSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Follows(s, 12)
Result* Follows::evaluateSynonymFixed(string leaderSynonym, string followerStmtNum, Type leaderType) {
	Result* result = new Result();
	int answer = pkb.getLeaderOf(stoi(followerStmtNum), leaderType);
	if (answer != -1) {
		result->setPassed(true);
		result->setAnswer(leaderSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Follows(_, a)
Result* Follows::evaluateUnderscoreSynonym(string followerSynonym, Type followerType) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getFollowerStmts(followerType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(followerSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Follows(_, _)
Result* Follows::evaluateUnderscoreUnderscore() {
	Result* result = new Result();
	unordered_set<int> leaders = pkb.getLeaderStmts(Type::STATEMENT);
	result->setPassed(!leaders.empty());
	return result;
}

// case Follows(_, 23)
Result* Follows::evaluateUnderscoreFixed(string followerStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.hasLeader(stoi(followerStmtNum)));
	return result;
}
