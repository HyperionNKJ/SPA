#include "FollowsT.h"

FollowsT::FollowsT(const DesignEntity& firstPara, const DesignEntity& secondPara) {
	paraOne = firstPara;
	paraTwo = secondPara;
	type = ClauseType::FOLLOWS_T;
	setSynonyms();
}

Result FollowsT::evaluate(const PKB& pkb) {
	this->pkb = pkb;
	Type paraOneType = paraOne.getType();
	Type paraTwoType = paraTwo.getType();
	std::string paraOneValue = paraOne.getValue();
	std::string paraTwoValue = paraTwo.getValue();

	Result result;

	if (paraOneType == FIXED) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT || paraTwoType == WHILE || paraTwoType == IF 
			|| paraTwoType == ASSIGN || paraTwoType == CALL || paraTwoType == PROGLINE || paraTwoType == SWITCH) {
			result = this->evaluateFixedSynonym(paraOneValue, paraTwoValue, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateFixedUnderscore(paraOneValue);
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateFixedFixed(paraOneValue, paraTwoValue);
		}
		else {
			result.setPassed(false);
		}
	}
	else if (paraOneType == STATEMENT || paraOneType == READ || paraOneType == PRINT || paraOneType == WHILE || paraOneType == IF 
		|| paraOneType == ASSIGN || paraOneType == CALL || paraOneType == PROGLINE || paraOneType == SWITCH) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT || paraTwoType == WHILE || paraTwoType == IF 
			|| paraTwoType == ASSIGN || paraTwoType == CALL || paraTwoType == PROGLINE || paraTwoType == SWITCH) {
			result = this->evaluateSynonymSynonym(paraOneValue, paraTwoValue, paraOneType, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateSynonymUnderscore(paraOneValue, paraOneType);
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateSynonymFixed(paraOneValue, paraTwoValue, paraOneType);
		}
		else {
			result.setPassed(false);
		}
	}
	else if (paraOneType == UNDERSCORE) {
		if (paraTwoType == STATEMENT || paraTwoType == READ || paraTwoType == PRINT || paraTwoType == WHILE || paraTwoType == IF 
			|| paraTwoType == ASSIGN || paraTwoType == CALL || paraTwoType == PROGLINE || paraTwoType == SWITCH) {
			result = this->evaluateUnderscoreSynonym(paraTwoValue, paraTwoType);
		}
		else if (paraTwoType == UNDERSCORE) {
			result = this->evaluateUnderscoreUnderscore();
		}
		else if (paraTwoType == FIXED) {
			result = this->evaluateUnderscoreFixed(paraTwoValue);
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

// case Follows*(12, w)
Result FollowsT::evaluateFixedSynonym(const std::string& leaderStmtNum, const std::string& followerSynonym, const Type& followerType) {
	Result result;
	std::unordered_set<int> answer = pkb.getFollowerTOf(stoi(leaderStmtNum), followerType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(followerSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Follows*(3, _)
Result FollowsT::evaluateFixedUnderscore(const std::string& leaderStmtNum) {
	Result result;
	result.setPassed(pkb.hasFollower(stoi(leaderStmtNum)));
	return result;
}

// case Follows*(4, 6)
Result FollowsT::evaluateFixedFixed(const std::string& leaderStmtNum, const std::string& followerStmtNum) {
	Result result;
	result.setPassed(pkb.isFollowsT(stoi(leaderStmtNum), stoi(followerStmtNum)));
	return result;
}

// case Follows*(r, a)
Result FollowsT::evaluateSynonymSynonym(const std::string& leaderSynonym, const std::string& followerSynonym, const Type& leaderType, const Type& followerType) {
	Result result;
	if (leaderSynonym == followerSynonym) {
		result.setPassed(false);
		return result;
	}
	std::unordered_map<int, std::unordered_set<int>> answer = pkb.getLeaderFollowerTPairs(leaderType, followerType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(leaderSynonym, followerSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Follows*(pr, _)
Result FollowsT::evaluateSynonymUnderscore(const std::string& leaderSynonym, const Type& leaderType) {
	Result result;
	std::unordered_set<int> answer = pkb.getLeaderStmts(leaderType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(leaderSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Follows*(s, 12)
Result FollowsT::evaluateSynonymFixed(const std::string& leaderSynonym, const std::string& followerStmtNum, const Type& leaderType) {
	Result result;
	std::unordered_set<int> answer = pkb.getLeaderTOf(stoi(followerStmtNum), leaderType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(leaderSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Follows*(_, a)
Result FollowsT::evaluateUnderscoreSynonym(const std::string& followerSynonym, const Type& followerType) {
	Result result;
	std::unordered_set<int> answer = pkb.getFollowerStmts(followerType);
	if (!answer.empty()) {
		result.setPassed(true);
		result.setAnswer(followerSynonym, answer);
	}
	else {
		result.setPassed(false);
	}
	return result;
}

// case Follows*(_, _)
Result FollowsT::evaluateUnderscoreUnderscore() {
	Result result;
	std::unordered_set<int> leaders = pkb.getLeaderStmts(Type::STATEMENT);
	result.setPassed(!leaders.empty());
	return result;
}

// case Follows*(_, 23)
Result FollowsT::evaluateUnderscoreFixed(const std::string& followerStmtNum) {
	Result result;
	result.setPassed(pkb.hasLeader(stoi(followerStmtNum)));
	return result;
}
