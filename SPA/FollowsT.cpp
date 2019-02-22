#include "FollowsT.h"

FollowsT::FollowsT(DesignEntity paraOne, DesignEntity paraTwo) : Clause(paraOne, paraTwo) {}

Result FollowsT::evaluate(PKB pkb) {
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

// case Follows*(12, w)
Result* FollowsT::evaluateFixedSynonym(string leaderStmtNum, string followerSynonym, Type followerType) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getFollowerTOf(stoi(leaderStmtNum), followerType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(followerSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Follows*(3, _)
Result* FollowsT::evaluateFixedUnderscore(string leaderStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.hasFollower(stoi(leaderStmtNum)));
	return result;
}

// case Follows*(4, 6)
Result* FollowsT::evaluateFixedFixed(string leaderStmtNum, string followerStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.isFollowsT(stoi(leaderStmtNum), stoi(followerStmtNum)));
	return result;
}

// case Follows*(r, a)
Result* FollowsT::evaluateSynonymSynonym(string leaderSynonym, string followerSynonym, Type leaderType, Type followerType) {
	Result* result = new Result();
	unordered_map<int, unordered_set<int>> answer = pkb.getLeaderFollowerTPairs(leaderType, followerType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(leaderSynonym, followerSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Follows*(pr, _)
Result* FollowsT::evaluateSynonymUnderscore(string leaderSynonym, Type leaderType) {
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

// case Follows*(s, 12)
Result* FollowsT::evaluateSynonymFixed(string leaderSynonym, string followerStmtNum, Type leaderType) {
	Result* result = new Result();
	unordered_set<int> answer = pkb.getLeaderTOf(stoi(followerStmtNum), leaderType);
	if (!answer.empty()) {
		result->setPassed(true);
		result->setAnswer(leaderSynonym, answer);
	}
	else {
		result->setPassed(false);
	}
	return result;
}

// case Follows*(_, a)
Result* FollowsT::evaluateUnderscoreSynonym(string followerSynonym, Type followerType) {
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

// case Follows*(_, _)
Result* FollowsT::evaluateUnderscoreUnderscore() {
	Result* result = new Result();
	unordered_set<int> leaders = pkb.getLeaderStmts(Type::STATEMENT);
	result->setPassed(!leaders.empty());
	return result;
}

// case Follows*(_, 23)
Result* FollowsT::evaluateUnderscoreFixed(string followerStmtNum) {
	Result* result = new Result();
	result->setPassed(pkb.hasLeader(stoi(followerStmtNum)));
	return result;
}
