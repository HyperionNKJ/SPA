#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "QueryProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
	PKB pkb;
	QueryProcessor queryProcessor;
	std::list<std::string> actualResults;
	std::list<std::string> expectedResults;

	TEST_CLASS(TestPKBToPQL) {
	
	public:
	TEST_CLASS_INITIALIZE(initialSetup) {
		// Code 5 in Full SPA Requirements. (only required relationships are inserted into PKB)
		
		pkb.insertStmtType(1, ASSIGN);
		pkb.insertStmtType(2, ASSIGN);
		pkb.insertStmtType(4, ASSIGN);
		pkb.insertStmtType(6, ASSIGN);
		pkb.insertStmtType(8, ASSIGN);
		pkb.insertStmtType(9, ASSIGN);
		pkb.insertStmtType(10, ASSIGN);
		pkb.insertStmtType(11, ASSIGN);
		pkb.insertStmtType(12, ASSIGN);
		pkb.insertStmtType(13, ASSIGN);
		pkb.insertStmtType(14, ASSIGN);

		pkb.insertStmtType(3, WHILE);
		pkb.insertCPRStmtType(5, CALL, "Third");
		pkb.insertStmtType(7, IF);
		pkb.insertCPRStmtType(15, PRINT, "v");
		pkb.insertCPRStmtType(16, READ, "x");
		pkb.insertCPRStmtType(17, READ, "z");
		pkb.insertCPRStmtType(18, CALL, "Second");

		pkb.insertProc("First");
		pkb.insertProc("Second");
		pkb.insertProc("Third");

		pkb.insertVar("x");
		pkb.insertVar("i");
		pkb.insertVar("z");
		pkb.insertVar("v");
		pkb.insertVar("y");

		pkb.insertConstant(0);
		pkb.insertConstant(5);
		pkb.insertConstant(2);
		pkb.insertConstant(1);

		// set Next
		pkb.setNext(1, 2);
		pkb.setPrevious(1, 2);
		pkb.setNext(2, 3);
		pkb.setPrevious(2, 3);
		pkb.setNext(3, 4);
		pkb.setPrevious(3, 4);
		pkb.setNext(3, 7);
		pkb.setPrevious(3, 7);
		pkb.setNext(4, 5);
		pkb.setPrevious(4, 5);
		pkb.setNext(5, 6);
		pkb.setPrevious(5, 6);
		pkb.setNext(6, 3);
		pkb.setPrevious(6, 3);
		pkb.setNext(7, 8);
		pkb.setPrevious(7, 8);
		pkb.setNext(7, 9);
		pkb.setPrevious(7, 9);
		pkb.setNext(8, 10);
		pkb.setPrevious(8, 10);
		pkb.setNext(9, 10);
		pkb.setPrevious(9, 10);
		pkb.setNext(10, 11);
		pkb.setPrevious(10, 11);
		pkb.setNext(11, 12);
		pkb.setPrevious(11, 12);
		pkb.setNext(13, 14);
		pkb.setPrevious(13, 14);
		pkb.setNext(14, 15);
		pkb.setPrevious(14, 15);
		pkb.setNext(16, 17);
		pkb.setPrevious(16, 17);
		pkb.setNext(17, 18);
		pkb.setPrevious(17, 18);

		// set Follows
		pkb.setFollows(1, 2);
		pkb.setFollowedBy(1, 2);
		pkb.setFollows(2, 3);
		pkb.setFollowedBy(2, 3);
		pkb.setFollows(3, 7);
		pkb.setFollowedBy(3, 7);
		pkb.setFollows(4, 5);
		pkb.setFollowedBy(4, 5);
		pkb.setFollows(5, 6);
		pkb.setFollowedBy(5, 6);
		pkb.setFollows(7, 10);
		pkb.setFollowedBy(7, 10);
		pkb.setFollows(10, 11);
		pkb.setFollowedBy(10, 11);
		pkb.setFollows(11, 12);
		pkb.setFollowedBy(11, 12);
		pkb.setFollows(13, 14);
		pkb.setFollowedBy(13, 14);
		pkb.setFollows(14, 15);
		pkb.setFollowedBy(14, 15);
		pkb.setFollows(16, 17);
		pkb.setFollowedBy(16, 17);
		pkb.setFollows(17, 18);
		pkb.setFollowedBy(17, 18);

		// only populate FollowsT for statement 3
		pkb.setFollowsT(3, 7);
		pkb.setFollowedByT(3, 7);
		pkb.setFollowsT(3, 10);
		pkb.setFollowedByT(3, 10);
		pkb.setFollowsT(3, 11);
		pkb.setFollowedByT(3, 11);
		pkb.setFollowsT(3, 12);
		pkb.setFollowedByT(3, 12);

		// set Parent/Parent*
		pkb.setParent(3, 4);
		pkb.setChild(3, 4);
		pkb.setParent(3, 5);
		pkb.setChild(3, 5);
		pkb.setParent(3, 6);
		pkb.setChild(3, 6);
		pkb.setParent(7, 8);
		pkb.setChild(7, 8);
		pkb.setParent(7, 9);
		pkb.setChild(7, 9);
		pkb.setParentT(3, 4);
		pkb.setChildT(3, 4);
		pkb.setParentT(3, 5);
		pkb.setChildT(3, 5);
		pkb.setParentT(3, 6);
		pkb.setChildT(3, 6);
		pkb.setParentT(7, 8);
		pkb.setChildT(7, 8);
		pkb.setParentT(7, 9);
		pkb.setChildT(7, 9);

		// only set pattern for statement 12
		pkb.insertFullPattern("x y * z +", 12);
		pkb.insertPattern("x y *", 12);
		pkb.insertPattern("x y * z +", 12);

		// set Modifies for statements
		pkb.setModifies(1, "x");
		pkb.setModifies(2, "i");
		pkb.setModifies(3, "x");
		pkb.setModifies(3, "i");
		pkb.setModifies(3, "z");
		pkb.setModifies(3, "v");
		pkb.setModifies(4, "x");
		pkb.setModifies(5, "z");
		pkb.setModifies(5, "v");
		pkb.setModifies(6, "i");
		pkb.setModifies(7, "x");
		pkb.setModifies(7, "z");
		pkb.setModifies(8, "x");
		pkb.setModifies(9, "z");
		pkb.setModifies(10, "z");
		pkb.setModifies(11, "y");
		pkb.setModifies(12, "x");
		pkb.setModifies(13, "z");
		pkb.setModifies(14, "v");
		pkb.setModifies(16, "x");
		pkb.setModifies(17, "z");
		pkb.setModifies(18, "i");
		pkb.setModifies(18, "x");
		pkb.setModifies(18, "y");
		pkb.setModifies(18, "z");
		pkb.setModifies(18, "v");

		// set Modifies for procedures
		pkb.setModifies("Third", "z");
		pkb.setModifies("Third", "v");
		pkb.setModifies("Second", "x");
		pkb.setModifies("Second", "i");
		pkb.setModifies("Second", "z");
		pkb.setModifies("Second", "v");
		pkb.setModifies("Second", "y");
		pkb.setModifies("First", "x");
		pkb.setModifies("First", "z");
		pkb.setModifies("First", "i");
		pkb.setModifies("First", "v");
		pkb.setModifies("First", "y");

		// set Uses for procedures
		pkb.setUses(3, "i");
		pkb.setUses(3, "x");
		pkb.setUses(3, "y");
		pkb.setUses(3, "z");
		pkb.setUses(3, "v");
		pkb.setUses(4, "x");
		pkb.setUses(4, "y");
		pkb.setUses(5, "z");
		pkb.setUses(5, "v");
		pkb.setUses(6, "i");
		pkb.setUses(7, "x");
		pkb.setUses(8, "x");
		pkb.setUses(10, "z");
		pkb.setUses(10, "x");
		pkb.setUses(10, "i");
		pkb.setUses(11, "z");
		pkb.setUses(12, "x");
		pkb.setUses(12, "y");
		pkb.setUses(12, "z");
		pkb.setUses(14, "z");
		pkb.setUses(15, "v");
		pkb.setUses(18, "i");
		pkb.setUses(18, "x");
		pkb.setUses(18, "y");
		pkb.setUses(18, "z");
		pkb.setUses(18, "v");

		// set Uses for procedures
		pkb.setUses("Third", "z");
		pkb.setUses("Third", "v");
		pkb.setUses("Second", "x");
		pkb.setUses("Second", "i");
		pkb.setUses("Second", "z");
		pkb.setUses("Second", "v");
		pkb.setUses("Second", "y");
		pkb.setUses("First", "x");
		pkb.setUses("First", "z");
		pkb.setUses("First", "i");
		pkb.setUses("First", "v");
		pkb.setUses("First", "y");

		// set Calls/Calls*
		pkb.setCalls("Second", "Third");
		pkb.setCalledBy("Second", "Third");
		pkb.setCalls("First", "Second");
		pkb.setCalledBy("First", "Second");	
		pkb.setCallsT("Second", "Third");
		pkb.setCalledByT("Second", "Third");
		pkb.setCallsT("First", "Second");
		pkb.setCalledByT("First", "Second");
		pkb.setCallsT("First", "Third");
		pkb.setCalledByT("First", "Third");
	}

	TEST_METHOD(TestSelect) {
		std::string query = "assign a; Select a";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = {"1", "2", "4", "6", "8", "9", "10", "11", "12", "13", "14"};
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestTuple) {
		std::string query = "read r; print pn; Select <r, r.varName, pn>";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "16 x 15", "17 z 15"};
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestPartialPattern) {
		std::string query = "assign a; variable v;  Select a pattern a(_, _\"x*y\"_)";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "12" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestFullPattern) {
		std::string query = "assign a; variable v;  Select a pattern a(_, \"x*y+z\")";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "12" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestFollows) {
		std::string query = "stmt s, s1; Select s such that Follows(s, s1)";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "1", "2", "3", "4", "5", "7", "10", "11", "13", "14", "16", "17" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestFollowsT) {
		std::string query = "stmt s; Select s such that Follows*(3, s)";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "7", "10", "11", "12" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestParent) {
		std::string query = "stmt s; Select s such that Parent(3, s)";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "4", "5", "6" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestParentT) {
		std::string query = "stmt s; Select s such that Parent*(s, 9)";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "7" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestUsesS) {
		std::string query = "stmt s; Select s such that Uses(s, \"x\")";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "3", "4", "7", "8", "10", "12", "18"};
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestUsesP) {
		std::string query = "variable v; Select v such that Uses(\"First\", v)";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "x", "y", "i", "z", "v" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestModifiesS) {
		std::string query = "stmt s; Select s such that Modifies(s, \"x\")";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "1", "3", "4", "7", "8", "12", "16", "18" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestModifiesP) {
		std::string query = "procedure p; Select p such that Modifies(p, \"x\")";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "First", "Second" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestCalls) {
		std::string query = "procedure p, p1; Select p1 such that Calls(p, p1)";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "Second", "Third" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestCallsT) {
		std::string query = "procedure p; Select p such that Calls*(\"First\", p)";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "Second", "Third" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestNext) {
		std::string query = "stmt s; Select s such that Next(3, s)";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "4", "7" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestNextT) {
		std::string query = "stmt s; Select s such that Next*(3, s)";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "4", "5", "6", "3", "7", "8", "9", "10", "11", "12" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestAffects) {
		std::string query = "stmt s; Select s such that Affects(1, s)";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "4", "8", "10", "12" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	TEST_METHOD(TestAffectsT) {
		std::string query = "stmt s; Select s such that Affects*(1, s)";
		actualResults = queryProcessor.evaluate(query, pkb);

		expectedResults = { "4", "8", "10", "11", "12" };
		Assert::IsTrue(isSameResult(expectedResults, actualResults));
	}

	// ---------------------------------- HELPER FUNCTIONS -----------------------------------------------
		// order in std::list does not matter
	bool isSameResult(std::list<std::string> expectedResults, std::list<std::string> actualResults) {
		if (expectedResults.size() != actualResults.size()) {
			return false;
		}

		std::unordered_set<std::string> expectedResultsSet;
		for (auto expectedResult : expectedResults) {
			expectedResultsSet.insert(expectedResult);
		}
		for (auto actualResult : actualResults) {
			if (expectedResultsSet.find(actualResult) == expectedResultsSet.end()) {
				return false;
			}
		}
		return true;
	}
	};
}