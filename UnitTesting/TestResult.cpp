#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/Result.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestResult) {
	public:

		bool isAnswerCorrect(unordered_map<string, list<int>> actual, unordered_map<string, list<int>> expected) {
			if (actual.size() != expected.size()) {
				return false;
			}
			auto actualItr = actual.begin();
			auto expectedItr = expected.begin();

			if (actual.size() == 1) {
				string actualKey = actualItr->first;
				list<int> actualValue = actualItr->second;
				string expectedKey = expectedItr->first;
				list<int> expectedValue = expectedItr->second;
				if (actualKey != expectedKey) {
					return false;
				}
				actualValue.sort();
				expectedValue.sort();
				if (actualValue != expectedValue) {
					return false;
				}
			}
			else if (actual.size() == 2) { // cannot simply sort because mapping will be distrupted
				string actualFirstKey = actualItr->first;
				list<int> actualFirstValue = actualItr->second;
				string expectedFirstKey = expectedItr->first;
				list<int> expectedFirstValue = expectedItr->second;
				actualItr++;
				expectedItr++;
				string actualSecondKey = actualItr->first;
				list<int> actualSecondValue = actualItr->second;
				string expectedSecondKey = expectedItr->first;
				list<int> expectedSecondValue = expectedItr->second;

				// firstly, check list sizes are all same
				if (actualFirstValue.size() != actualSecondValue.size() || actualFirstValue.size() != expectedFirstValue.size() || actualFirstValue.size() != expectedSecondValue.size()) {
					return false;
				}

				// secondly, check keys equality and swap to simplify 
				if (actualFirstKey == expectedFirstKey && actualSecondKey == expectedSecondKey) {
					// no swapping required.
				}
				else if (actualFirstKey == expectedSecondKey && actualSecondKey == expectedFirstKey) {
					string temp = actualFirstKey;
					actualFirstKey = actualSecondKey;
					actualSecondKey = temp;
					list<int> tempList = actualFirstValue;
					actualFirstValue = actualSecondValue;
					actualSecondValue = tempList;
				}
				else {
					return false;
				}

				// thirdly, check if mappings are correct
				unordered_multimap<int, int> actualMapping;
				unordered_multimap<int, int> expectedMapping;
				auto actualFirstItr = actualFirstValue.begin();
				auto actualSecondItr = actualSecondValue.begin();
				auto expectedFirstItr = expectedFirstValue.begin();
				auto expectedSecondItr = expectedSecondValue.begin();

				while (actualFirstItr != actualFirstValue.end()) {
					pair<int, int> actualMap(*actualFirstItr, *actualSecondItr);
					pair<int, int> expectedMap(*expectedFirstItr, *expectedSecondItr);
					actualMapping.insert(actualMap);
					expectedMapping.insert(expectedMap);
					actualFirstItr++;
					actualSecondItr++;
					expectedFirstItr++;
					expectedSecondItr++;
				}

				if (actualMapping != expectedMapping) {
					return false;
				}
			}
			else { // if actual.size() == 3 or more, return false because impossible
				return false;
			}
			return true;
		}
		TEST_METHOD(setAnswer_setOfString) {
			Result result;

			string variableSynonym = "v";
			unordered_set<string> variables({ "this", "is", "a", "test" });
			unordered_map<string, int> varToIndexTable({ {"hello", 0}, {"my", 1}, {"name", 2},
				{"is", 3}, {"spa", 4}, {"testing", 5},
				{"currently", 6}, {"in", 7}, {"progress", 8},
				{"this", 9}, {"a", 11}, {"test", 12} });
			result.setAnswer(variableSynonym, variables, varToIndexTable);

			unordered_map<string, list<int>> expectedAnswer({ {"v", { 3,9,11,12 }} });
			unordered_map<string, list<int>> actualAnswer = result.getAnswer();
			Assert::IsTrue(isAnswerCorrect(actualAnswer, expectedAnswer));
		}

		TEST_METHOD(setAnswer_mapOfIntAndStringSet) {
			Result result;

			string stmtSynonym = "a";
			string variableSynonym = "v";
			unordered_map<int, unordered_set<string>> answer({ {4, {"progress", "so"}}, {7, {"spa","set"}}, {9, {"random"}} });
			unordered_map<string, int> varToIndexTable({ {"hello", 0}, {"is", 1}, {"name", 2},
				{"convenient", 3}, {"spa", 4}, {"so", 5},
				{"set", 6}, {"in", 7}, {"progress", 8},
				{"this", 9}, {"random", 11}, {"unordered", 12} });

			unordered_map<string, list<int>> expectedAnswer({ {"a", {4,4,7,7,9}}, {"v", {8,5,6,4,11}} });

			result.setAnswer(stmtSynonym, variableSynonym, answer, varToIndexTable);
			unordered_map<string, list<int>> actualAnswer = result.getAnswer();
			Assert::IsTrue(isAnswerCorrect(actualAnswer, expectedAnswer));
		}
		TEST_METHOD(setAnswer_mapOfIntAndIntSet) {
			Result result;

			string stmt1Synonym = "w";
			string stmt2Synonym = "a";
			unordered_map<int, unordered_set<int>> answer({ {4, {6,7,8}}, {14, {15,16}}, {22, {24}} });

			unordered_map<string, list<int>> expectedAnswer({ {"w", {4,4,4,14,14,22}}, {"a", {6,7,8,15,16,24}} });

			result.setAnswer(stmt1Synonym, stmt2Synonym, answer);
			unordered_map<string, list<int>> actualAnswer = result.getAnswer();
			Assert::IsTrue(isAnswerCorrect(actualAnswer, expectedAnswer));
		}

		TEST_METHOD(setAnswer_setOfInt) {
			Result result;

			string stmt1Synonym = "a";
			unordered_set<int> answer({ 1,2,4,6,8 });

			unordered_map<string, list<int>> expectedAnswer({ {"a", {1,2,4,6,8}} });

			result.setAnswer(stmt1Synonym, answer);
			unordered_map<string, list<int>> actualAnswer = result.getAnswer();
			Assert::IsTrue(isAnswerCorrect(actualAnswer, expectedAnswer));
		}

		TEST_METHOD(setAnswer_int) {
			Result result;

			string stmtSynonym = "w";
			int answer = 3;

			unordered_map<string, list<int>> expectedAnswer({ {"w", {3}} });

			result.setAnswer(stmtSynonym, answer);
			unordered_map<string, list<int>> actualAnswer = result.getAnswer();
			Assert::IsTrue(isAnswerCorrect(actualAnswer, expectedAnswer));
		}

		TEST_METHOD(setAnswer_mapOfIntAndInt) {
			Result result;

			string stmt1Synonym = "read";
			string stmt2Synonym = "assign";
			unordered_map<int, int> answer({ {4,5}, {5,6}, {6,7}, {13,14}, {14,15}, {22,23} });

			unordered_map<string, list<int>> expectedAnswer({ {"read", {4,5,6,13,14,22}}, {"assign", {5,6,7,14,15,23}} });

			result.setAnswer(stmt1Synonym, stmt2Synonym, answer);
			unordered_map<string, list<int>> actualAnswer = result.getAnswer();
			Assert::IsTrue(isAnswerCorrect(actualAnswer, expectedAnswer));
		}

		TEST_METHOD(setAnswer_mapOfIntAndString) {
			Result result;

			string stmtSynonym = "a";
			string variableSynonym = "v";
			unordered_map<int, string> answer({ {4, "h"}, {5, "a"}, {6,"count"}, {13,"the"}, {14,"one"} });
			unordered_map<string, int> varToIndexTable({ {"h", 0}, {"is", 1}, {"name", 2},
					{"convenient", 3}, {"spa", 4}, {"a", 5},
					{"one", 6}, {"count", 7}, {"progress", 8},
					{"the", 9}, {"random", 11}, {"unordered", 12} });

			unordered_map<string, list<int>> expectedAnswer({ {"a", {4,14,6,13,5}}, {"v", {0,6,7,9,5}} });

			result.setAnswer(stmtSynonym, variableSynonym, answer, varToIndexTable);
			unordered_map<string, list<int>> actualAnswer = result.getAnswer();
			Assert::IsTrue(isAnswerCorrect(actualAnswer, expectedAnswer));
		}
	};
}