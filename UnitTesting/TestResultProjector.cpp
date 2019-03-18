#include "stdafx.h"
#include "CppUnitTest.h"
#include "ResultProjector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestResultProjector) {
	public:

		/* Convention:
			OneOne -> 1 synonym from first clause, 1 synonym from second clause
			OneTwo -> 1 synonym from first clause, 2 synonyms from second clause
			TwoOne -> 2 synonyms from first clause, 1 synonym from second clause
			TwoTwo -> 2 synonyms from first clause, 2 synonyms from second clause */

		// OneOne
		TEST_METHOD(combineOneOneWithNoCommonSynonym) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym { "a" };
			vector<string> result2Synonym { "b" };

			unordered_set<int> result1;
			unordered_set<int> result2;
			result1 = { 9,3,5,2 };
			result2 = { 4,5,3 };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 1;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults1;
			expectedResults1.push_back({ {"a", 9} });
			expectedResults1.push_back({ {"a", 3} });
			expectedResults1.push_back({ {"a", 5} });
			expectedResults1.push_back({ {"a", 2} });
			expectedSynonymResults[0] = expectedResults1;

			list<unordered_map<string, int>> expectedResults2;
			expectedResults2.push_back({ {"b", 4} });
			expectedResults2.push_back({ {"b", 5} });
			expectedResults2.push_back({ {"b", 3} });
			expectedSynonymResults[1] = expectedResults2;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneOneWithOneCommonSynonym) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a" };
			vector<string> result2Synonym{ "a" };

			unordered_set<int> result1;
			unordered_set<int> result2;
			result1 = { 9,3,5,2 };
			result2 = { 4,5,3 };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;


			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 3} });
			expectedResults.push_back({ {"a", 5} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneOneWithOneCommonSynonymWithNoCommonResults) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a" };
			vector<string> result2Synonym{ "a" };

			unordered_set<int> result1;
			unordered_set<int> result2;
			result1 = { 9,3,5,2 };
			result2 = { 4 };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		// OneTwo
		TEST_METHOD(combineOneTwoWithNoCommonSynonym) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a" };
			vector<string> result2Synonym{ "b", "c" };

			unordered_set<int> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { 9,3,5,2 };
			result2 = {	{1, {4,6}},
						{2, {5}},
						{3, {3}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 1;
			expectedSynonymTable["c"] = 1;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults1;
			expectedResults1.push_back({ {"a", 9} });
			expectedResults1.push_back({ {"a", 3} });
			expectedResults1.push_back({ {"a", 5} });
			expectedResults1.push_back({ {"a", 2} });
			expectedSynonymResults[0] = expectedResults1;

			list<unordered_map<string, int>> expectedResults2;
			expectedResults2.push_back({ {"b", 1}, {"c", 4} });
			expectedResults2.push_back({ {"b", 1}, {"c", 6} });
			expectedResults2.push_back({ {"b", 2}, {"c", 5} });
			expectedResults2.push_back({ {"b", 3}, {"c", 3} });
			expectedSynonymResults[1] = expectedResults2;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneTwoWithOneCommonSynonym) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a" };
			vector<string> result2Synonym{ "b", "a" };

			unordered_set<int> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { 9,3,5,2 };
			result2 = { {1, {4}},
						{2, {5,3}},
						{3, {3}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 3}, {"b", 2}});
			expectedResults.push_back({ {"a", 3}, {"b", 3}});
			expectedResults.push_back({ {"a", 5}, {"b", 2}});
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneTwoWithOneCommonSynonym2) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "b" };
			vector<string> result2Synonym{ "b", "a" };

			unordered_set<int> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { 2,5,6 };
			result2 = { {2, {1,5}},
						{5, {2}},
						{1, {4}},
						{8, {7}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 1}, {"b", 2} });
			expectedResults.push_back({ {"a", 5}, {"b", 2} });
			expectedResults.push_back({ {"a", 2}, {"b", 5} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneTwoWithOneCommonSynonym3) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a" };
			vector<string> result2Synonym{ "a", "b" };

			unordered_set<int> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { 1,2,4,8 };
			result2 = { {1, {2}},
						{2, {5,6,7}},
						{3, {6}},
						{4, {1,4}},
						{5, {2}},
						{7, {8}}, };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 1}, {"b", 2} });
			expectedResults.push_back({ {"a", 2}, {"b", 5} });
			expectedResults.push_back({ {"a", 2}, {"b", 6} });
			expectedResults.push_back({ {"a", 2}, {"b", 7} });
			expectedResults.push_back({ {"a", 4}, {"b", 1} });
			expectedResults.push_back({ {"a", 4}, {"b", 4} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneTwoWithOneCommonSynonymWithNoCommonResults) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a" };
			vector<string> result2Synonym{ "a", "b" };

			unordered_set<int> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { 9,3,5,2 };
			result2 = { {1, {4}},
						{8, {6}},
						{6, {2}},
						{4, {1}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		// TwoOne
		TEST_METHOD(combineTwoOneWithNoCommonSynonym) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "c" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_set<int> result2;
			result1 = { {9, {1}},
						{3, {2}},
						{5, {3}},
						{2, {4}} };
			result2 = { 4,5,3 };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 1;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults1;
			expectedResults1.push_back({ {"a", 9}, {"b", 1} });
			expectedResults1.push_back({ {"a", 3}, {"b", 2} });
			expectedResults1.push_back({ {"a", 5}, {"b", 3} });
			expectedResults1.push_back({ {"a", 2}, {"b", 4} });
			expectedSynonymResults[0] = expectedResults1;

			list<unordered_map<string, int>> expectedResults2;
			expectedResults2.push_back({ {"c", 4} });
			expectedResults2.push_back({ {"c", 5} });
			expectedResults2.push_back({ {"c", 3} });
			expectedSynonymResults[1] = expectedResults2;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoOneWithOneCommonSynonym) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "b" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_set<int> result2;
			result1 = { {9, {1}},
						{3, {2}},
						{5, {3}},
						{2, {4}} };
			result2 = { 3,1 };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 9}, {"b", 1} });
			expectedResults.push_back({ {"a", 5}, {"b", 3} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoOneWithOneCommonSynonymWithNoCommonResults) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "b" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_set<int> result2;
			result1 = { {9, {1}},
						{3, {5}},
						{5, {3}},
						{2, {4}} };
			result2 = { 2,6 };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		// TwoTwo
		TEST_METHOD(combineTwoTwoWithNoCommonSynonym) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "c", "d" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {9, {1}},
						{3, {2}},
						{5, {3}},
						{2, {4}} };
			result2 = { {4, {2}},
						{5, {5}},
						{3, {3}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 1;
			expectedSynonymTable["d"] = 1;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults1;
			expectedResults1.push_back({ {"a", 9}, {"b", 1} });
			expectedResults1.push_back({ {"a", 3}, {"b", 2} });
			expectedResults1.push_back({ {"a", 5}, {"b", 3} });
			expectedResults1.push_back({ {"a", 2}, {"b", 4} });
			expectedSynonymResults[0] = expectedResults1;

			list<unordered_map<string, int>> expectedResults2;
			expectedResults2.push_back({ {"c", 4}, {"d", 2} });
			expectedResults2.push_back({ {"c", 5}, {"d", 5} });
			expectedResults2.push_back({ {"c", 3}, {"d", 3} });
			expectedSynonymResults[1] = expectedResults2;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithOneCommonSynonym) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "b", "c" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {9, {1}},
						{3, {5}},
						{5, {3}},
						{2, {4}} };
			result2 = { {5, {6}},
						{2, {9}},
						{1, {2}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 9}, {"b", 1}, {"c", 2} });
			expectedResults.push_back({ {"a", 3}, {"b", 5}, {"c", 6} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithOneCommonSynonym2) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "a", "c" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {9, {1}},
						{3, {5}},
						{5, {3}},
						{2, {4}} };
			result2 = { {5, {6,7}},
						{2, {9,1,4}},
						{1, {2,3}},
						{4, {2}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 5}, {"b", 3}, {"c", 6} });
			expectedResults.push_back({ {"a", 5}, {"b", 3}, {"c", 7} });
			expectedResults.push_back({ {"a", 2}, {"b", 4}, {"c", 9} });
			expectedResults.push_back({ {"a", 2}, {"b", 4}, {"c", 1} });
			expectedResults.push_back({ {"a", 2}, {"b", 4}, {"c", 4} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithOneCommonSynonym3) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "b", "c" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {9, {1}},
						{3, {5}},
						{5, {3}},
						{2, {4}} };
			result2 = { {5, {6,7,1}},
						{2, {9}},
						{1, {2,3}},
						{4, {2}},
						{9, {2}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 9}, {"b", 1}, {"c", 2} });
			expectedResults.push_back({ {"a", 9}, {"b", 1}, {"c", 3} });
			expectedResults.push_back({ {"a", 3}, {"b", 5}, {"c", 6} });
			expectedResults.push_back({ {"a", 3}, {"b", 5}, {"c", 7} });
			expectedResults.push_back({ {"a", 3}, {"b", 5}, {"c", 1} });
			expectedResults.push_back({ {"a", 2}, {"b", 4}, {"c", 2} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithOneCommonSynonym4) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "b", "c" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {3, {0,3}},
						{6, {3,1}},
						{9, {2,0,3,1}} };
			result2 = { {0, {4}},
						{3, {7,14}},
						{2, {11}},
						{1, {14}},
						{4, {16}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 3}, {"b", 0}, {"c", 4} });
			expectedResults.push_back({ {"a", 3}, {"b", 3}, {"c", 7} });
			expectedResults.push_back({ {"a", 3}, {"b", 3}, {"c", 14} });
			expectedResults.push_back({ {"a", 6}, {"b", 3}, {"c", 7} });
			expectedResults.push_back({ {"a", 6}, {"b", 3}, {"c", 14} });
			expectedResults.push_back({ {"a", 6}, {"b", 1}, {"c", 14} });
			expectedResults.push_back({ {"a", 9}, {"b", 2}, {"c", 11} });
			expectedResults.push_back({ {"a", 9}, {"b", 0}, {"c", 4} });
			expectedResults.push_back({ {"a", 9}, {"b", 3}, {"c", 7} });
			expectedResults.push_back({ {"a", 9}, {"b", 3}, {"c", 14} });
			expectedResults.push_back({ {"a", 9}, {"b", 1}, {"c", 14} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithOneCommonSynonymWithAllCommonResults) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "c", "b" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {9, {1}},
						{3, {5}},
						{5, {3}},
						{2, {4}}, };
			result2 = { {2, {1,2}},
						{7, {5}},
						{6, {3}},
						{1, {4}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 9}, {"b", 1}, {"c", 2} });
			expectedResults.push_back({ {"a", 3}, {"b", 5}, {"c", 7} });
			expectedResults.push_back({ {"a", 5}, {"b", 3}, {"c", 6} });
			expectedResults.push_back({ {"a", 2}, {"b", 4}, {"c", 1} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithOneCommonSynonymWithAllCommonResults2) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "b", "c" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {9, {1}},
						{3, {3}},
						{5, {4}} };
			result2 = { {1, {2,3}},
						{3, {4,5}},
						{4, {6,7}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 9}, {"b", 1}, {"c", 2} });
			expectedResults.push_back({ {"a", 9}, {"b", 1}, {"c", 3} });
			expectedResults.push_back({ {"a", 3}, {"b", 3}, {"c", 4} });
			expectedResults.push_back({ {"a", 3}, {"b", 3}, {"c", 5} });
			expectedResults.push_back({ {"a", 5}, {"b", 4}, {"c", 6} });
			expectedResults.push_back({ {"a", 5}, {"b", 4}, {"c", 7} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithOneCommonSynonymWithNoCommonResults) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "a", "c" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {9, {1}},
						{3, {5}},
						{5, {3}},
						{2, {4}}, };
			result2 = { {1, {2}},
						{4, {6}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		TEST_METHOD(combineTwoTwoWithTwoCommonSynonym) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "b", "a" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {9, {1}},
						{3, {5}},
						{5, {3}},
						{2, {4}} };
			result2 = { {2, {3}},
						{4, {2}},
						{1, {7}},
						{5, {3}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 3}, {"b", 5} });
			expectedResults.push_back({ {"a", 2}, {"b", 4} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithTwoCommonSynonym2) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "a", "b" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {1, {1}},
						{2, {3,4}},
						{4, {5}} };
			result2 = { {2, {5,3}},
						{3, {1}},
						{4, {5,6}},
						{5, {1}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 2}, {"b", 3} });
			expectedResults.push_back({ {"a", 4}, {"b", 5} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithTwoCommonSynonym3) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "a", "b" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {2, {5,3}},
						{3, {1}},
						{4, {5,6}},
						{5, {1}}, };
			result2 = { {1, {1}},
						{4, {5}},
						{2, {4,3}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 2}, {"b", 3} });
			expectedResults.push_back({ {"a", 4}, {"b", 5} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithTwoCommonSynonymWithAllCommonResults) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "a", "b" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {1, {1}},
						{2, {3,4}},
						{4, {5}} };
			result2 = { {2, {4,3}},
						{1, {1}},
						{4, {5}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 1}, {"b", 1} });
			expectedResults.push_back({ {"a", 2}, {"b", 3} });
			expectedResults.push_back({ {"a", 2}, {"b", 4} });
			expectedResults.push_back({ {"a", 4}, {"b", 5} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithTwoCommonSynonymWithNoCommonResults) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "a", "b" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {1, {1}},
						{2, {3,4}},
						{4, {5}} };
			result2 = { {2, {1,2}},
						{1, {2}},
						{4, {4}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithTwoCommonSynonymWithNoCommonResults2) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "a", "b" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			result1 = { {9, {1}},
						{3, {3}},
						{5, {5}},
						{1, {2}}, 
						{2, {5,1}} };
			result2 = { {1, {1}},
						{2, {3,4}},
						{4, {5}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoTwoWithOneCommonSynonymSameTable) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "b", "c" };
			vector<string> result3Synonym{ "c", "d" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			unordered_map<int, unordered_set<int>> result3;
			result1 = { {2, {1,3}},
						{5, {4,6}},
						{6, {2,8,3}} };
			result2 = { {1, {2}},
						{2, {3,9}},
						{4, {7,4,1}},
						{8, {6}} };
			result3 = { {1, {2}},
						{2, {5}},
						{3, {6}},
						{4, {1,8}},
						{7, {8}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);
			resultProjector.combineResults(result3, result3Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;
			expectedSynonymTable["d"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 2}, {"b", 1}, {"c", 2}, {"d", 5} });
			expectedResults.push_back({ {"a", 5}, {"b", 4}, {"c", 7}, {"d", 8} });
			expectedResults.push_back({ {"a", 5}, {"b", 4}, {"c", 4}, {"d", 1} });
			expectedResults.push_back({ {"a", 5}, {"b", 4}, {"c", 4}, {"d", 8} });
			expectedResults.push_back({ {"a", 5}, {"b", 4}, {"c", 1}, {"d", 2} });
			expectedResults.push_back({ {"a", 6}, {"b", 2}, {"c", 3}, {"d", 6} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoTwoWithOneCommonSynonymSameTable2) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "c", "b" };
			vector<string> result3Synonym{ "c", "d" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			unordered_map<int, unordered_set<int>> result3;
			result1 = { {2, {1,3}},
						{5, {4,6}},
						{6, {2,8,3}} };
			result2 = { {2, {1}},
						{3, {2}},
						{9, {2}},
						{7, {4}},
						{4, {4}},
						{1, {4}},
						{6, {8}} };
			result3 = { {1, {2}},
						{2, {5}},
						{3, {6}},
						{4, {1,8}},
						{7, {8}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);
			resultProjector.combineResults(result3, result3Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;
			expectedSynonymTable["d"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 2}, {"b", 1}, {"c", 2}, {"d", 5} });
			expectedResults.push_back({ {"a", 5}, {"b", 4}, {"c", 1}, {"d", 2} });
			expectedResults.push_back({ {"a", 5}, {"b", 4}, {"c", 7}, {"d", 8} });
			expectedResults.push_back({ {"a", 5}, {"b", 4}, {"c", 4}, {"d", 1} });
			expectedResults.push_back({ {"a", 5}, {"b", 4}, {"c", 4}, {"d", 8} });
			expectedResults.push_back({ {"a", 6}, {"b", 2}, {"c", 3}, {"d", 6} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoTwoWithOneCommonSynonymSameTable3) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "a", "b" };
			vector<string> result2Synonym{ "a", "c" };
			vector<string> result3Synonym{ "b", "d" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			unordered_map<int, unordered_set<int>> result3;
			result1 = { {2, {1,3}},
						{4, {4,6}},
						{6, {2,8,3}} };
			result2 = { {1, {2}},
						{2, {3,9}},
						{4, {7,4,1}},
						{8, {6}} };
			result3 = { {1, {2}},
						{2, {5}},
						{3, {6}},
						{4, {1,8}},
						{7, {8}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);
			resultProjector.combineResults(result3, result3Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;
			expectedSynonymTable["d"] = 0;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 2}, {"b", 1}, {"c", 3}, {"d", 2} });
			expectedResults.push_back({ {"a", 2}, {"b", 1}, {"c", 9}, {"d", 2} });
			expectedResults.push_back({ {"a", 2}, {"b", 3}, {"c", 3}, {"d", 6} });
			expectedResults.push_back({ {"a", 2}, {"b", 3}, {"c", 9}, {"d", 6} });
			expectedResults.push_back({ {"a", 4}, {"b", 4}, {"c", 7}, {"d", 1} });
			expectedResults.push_back({ {"a", 4}, {"b", 4}, {"c", 7}, {"d", 8} });
			expectedResults.push_back({ {"a", 4}, {"b", 4}, {"c", 4}, {"d", 1} });
			expectedResults.push_back({ {"a", 4}, {"b", 4}, {"c", 4}, {"d", 8} });
			expectedResults.push_back({ {"a", 4}, {"b", 4}, {"c", 1}, {"d", 1} });
			expectedResults.push_back({ {"a", 4}, {"b", 4}, {"c", 1}, {"d", 8} });
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		// testing merging different tables
		TEST_METHOD(combineOneOneTwoWithTwoCommonSynonymDiffTable) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "b" };
			vector<string> result2Synonym{ "a" };
			vector<string> result3Synonym{ "a", "b" };

			unordered_set<int> result1;
			unordered_set<int> result2;
			unordered_map<int, unordered_set<int>> result3;
			result1 = { 2,5,6 };
			result2 = { 1,2,4,5,6,7 };
			result3 = { {1, {2}},
						{2, {5}},
						{3, {6}},
						{4, {1}},
						{5, {2}},
						{7, {8}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);
			resultProjector.combineResults(result3, result3Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 2;
			expectedSynonymTable["b"] = 2;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 1}, {"b", 2} });
			expectedResults.push_back({ {"a", 2}, {"b", 5} });
			expectedResults.push_back({ {"a", 5}, {"b", 2} });
			expectedSynonymResults[2] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneTwoTwoWithTwoCommonSynonymDiffTable) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "b" };
			vector<string> result2Synonym{ "a", "c" };
			vector<string> result3Synonym{ "a", "b" };

			unordered_set<int> result1;
			unordered_map<int, unordered_set<int>> result2;
			unordered_map<int, unordered_set<int>> result3;
			result1 = { 2,5,6 };
			result2 = { {1, {2}},
						{2, {3,9}},
						{4, {7,4,1}},
						{8, {6}} };
			result3 = { {1, {2}},
						{2, {5}},
						{3, {6}},
						{4, {1}},
						{5, {2}},
						{7, {8}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);
			resultProjector.combineResults(result3, result3Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 2;
			expectedSynonymTable["b"] = 2;
			expectedSynonymTable["c"] = 2;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 1}, {"b", 2}, {"c", 2} });
			expectedResults.push_back({ {"a", 2}, {"b", 5}, {"c", 3} });
			expectedResults.push_back({ {"a", 2}, {"b", 5}, {"c", 9} });
			expectedSynonymResults[2] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoTwoWithTwoCommonSynonymDiffTable) {
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			vector<string> result1Synonym{ "b", "d" };
			vector<string> result2Synonym{ "a", "c" };
			vector<string> result3Synonym{ "a", "b" };

			unordered_map<int, unordered_set<int>> result1;
			unordered_map<int, unordered_set<int>> result2;
			unordered_map<int, unordered_set<int>> result3;
			result1 = { {2, {1,3}},
						{5, {4,6}},
						{6, {2,8,3}} };
			result2 = { {1, {2}},
						{2, {3,9}},
						{4, {7,4,1}},
						{8, {6}} };
			result3 = { {1, {2}},
						{2, {5}},
						{3, {6}},
						{4, {1}},
						{5, {2}},
						{7, {8}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);
			resultProjector.combineResults(result3, result3Synonym);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 2;
			expectedSynonymTable["b"] = 2;
			expectedSynonymTable["c"] = 2;
			expectedSynonymTable["d"] = 2;

			unordered_map<int, list<unordered_map<string, int>>> expectedSynonymResults;
			list<unordered_map<string, int>> expectedResults;
			expectedResults.push_back({ {"a", 1}, {"b", 2}, {"c", 2}, {"d", 1} });
			expectedResults.push_back({ {"a", 1}, {"b", 2}, {"c", 2}, {"d", 3} });
			expectedResults.push_back({ {"a", 2}, {"b", 5}, {"c", 3}, {"d", 4} });
			expectedResults.push_back({ {"a", 2}, {"b", 5}, {"c", 3}, {"d", 6} });
			expectedResults.push_back({ {"a", 2}, {"b", 5}, {"c", 9}, {"d", 4} });
			expectedResults.push_back({ {"a", 2}, {"b", 5}, {"c", 9}, {"d", 6} });
			expectedSynonymResults[2] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		// -----------------------------------------------------------------------------------------------------
		// test select
		TEST_METHOD(selectOneSynonym) {
			ResultProjector resultProjector;
			// set up
			static unordered_map<string, int> synonymTable;
			static unordered_map<int, list<unordered_map<string, int>>> synonymResults;

			synonymTable["a"] = 0;
			synonymTable["b"] = 0;
			synonymTable["c"] = 0;

			list<unordered_map<string, int>> synonymResult;
			synonymResult.push_back({ {"a", 1}, {"b", 2}, {"c", 2} });
			synonymResult.push_back({ {"a", 3}, {"b", 1}, {"c", 1} });
			synonymResult.push_back({ {"a", 7}, {"b", 2}, {"c", 9} });
			synonymResult.push_back({ {"a", 2}, {"b", 4}, {"c", 5} });
			synonymResult.push_back({ {"a", 5}, {"b", 5}, {"c", 7} });
			synonymResults[0] = synonymResult;

			resultProjector.setSynonymTable(synonymTable);
			resultProjector.setSynonymResults(synonymResults);

			vector<DesignEntity> selectedSynonyms = { DesignEntity("a", Type::STATEMENT) };
			PKB pkb; // dummy

			// get results
			list<string> actualResults = resultProjector.getResults(selectedSynonyms, pkb);
			list<string> expectedResults = { "1", "3", "7", "2", "5" };

			Assert::IsTrue(isSameResult(expectedResults, actualResults));
		}
		TEST_METHOD(selectTupleSameTable) {
			ResultProjector resultProjector;
			// set up
			static unordered_map<string, int> synonymTable;
			static unordered_map<int, list<unordered_map<string, int>>> synonymResults;

			synonymTable["a"] = 0;
			synonymTable["b"] = 0;
			synonymTable["c"] = 0;

			list<unordered_map<string, int>> synonymResult;
			synonymResult.push_back({ {"a", 1}, {"b", 2}, {"c", 2} });
			synonymResult.push_back({ {"a", 3}, {"b", 1}, {"c", 1} });
			synonymResult.push_back({ {"a", 7}, {"b", 2}, {"c", 9} });
			synonymResult.push_back({ {"a", 2}, {"b", 4}, {"c", 5} });
			synonymResult.push_back({ {"a", 5}, {"b", 5}, {"c", 7} });
			synonymResults[0] = synonymResult;

			resultProjector.setSynonymTable(synonymTable);
			resultProjector.setSynonymResults(synonymResults);

			vector<DesignEntity> selectedSynonyms = {	DesignEntity("b", Type::STATEMENT),
														DesignEntity("a", Type::ASSIGN) };
			PKB pkb; // dummy
			
			// get results
			list<string> actualResults = resultProjector.getResults(selectedSynonyms, pkb);
			list<string> expectedResults = { "2 1", "1 3", "2 7", "4 2", "5 5" };

			Assert::IsTrue(isSameResult(expectedResults, actualResults));
		}
		TEST_METHOD(selectTupleDiffTable) {
			ResultProjector resultProjector;
			// set up
			static unordered_map<string, int> synonymTable;
			static unordered_map<int, list<unordered_map<string, int>>> synonymResults;

			synonymTable["a"] = 0;
			synonymTable["b"] = 0;
			synonymTable["c"] = 1;
			synonymTable["d"] = 1;

			list<unordered_map<string, int>> synonymResult1;
			synonymResult1.push_back({ {"a", 1}, {"b", 2} });
			synonymResult1.push_back({ {"a", 3}, {"b", 1} });
			synonymResult1.push_back({ {"a", 7}, {"b", 2} });
			synonymResults[0] = synonymResult1;
			list<unordered_map<string, int>> synonymResult2;
			synonymResult2.push_back({ {"c", 11}, {"d", 22} });
			synonymResult2.push_back({ {"c", 33}, {"d", 11} });
			synonymResult2.push_back({ {"c", 77}, {"d", 22} });
			synonymResults[1] = synonymResult2;

			resultProjector.setSynonymTable(synonymTable);
			resultProjector.setSynonymResults(synonymResults);

			vector<DesignEntity> selectedSynonyms = {	DesignEntity("b", Type::STATEMENT),
														DesignEntity("c", Type::ASSIGN) };
			PKB pkb; // dummy

			// get results
			list<string> actualResults = resultProjector.getResults(selectedSynonyms, pkb);
			list<string> expectedResults = {	"2 11", "2 33", "2 77",
												"1 11", "1 33", "1 77",
												"2 11", "2 33", "2 77" };

			Assert::IsTrue(isSameResult(expectedResults, actualResults));
		}
		TEST_METHOD(selectTupleDiffTable2) {
			ResultProjector resultProjector;
			// set up
			static unordered_map<string, int> synonymTable;
			static unordered_map<int, list<unordered_map<string, int>>> synonymResults;

			synonymTable["a"] = 0;
			synonymTable["b"] = 0;
			synonymTable["c"] = 1;
			synonymTable["d"] = 1;

			list<unordered_map<string, int>> synonymResult1;
			synonymResult1.push_back({ {"a", 1}, {"b", 2} });
			synonymResult1.push_back({ {"a", 3}, {"b", 1} });
			synonymResult1.push_back({ {"a", 7}, {"b", 2} });
			synonymResults[0] = synonymResult1;
			list<unordered_map<string, int>> synonymResult2;
			synonymResult2.push_back({ {"c", 11}, {"d", 22} });
			synonymResult2.push_back({ {"c", 33}, {"d", 11} });
			synonymResult2.push_back({ {"c", 77}, {"d", 22} });
			synonymResults[1] = synonymResult2;

			resultProjector.setSynonymTable(synonymTable);
			resultProjector.setSynonymResults(synonymResults);

			vector<DesignEntity> selectedSynonyms = {	DesignEntity("b", Type::STATEMENT),
														DesignEntity("c", Type::ASSIGN),
														DesignEntity("a", Type::ASSIGN) };
			PKB pkb; // dummy

			// get results
			list<string> actualResults = resultProjector.getResults(selectedSynonyms, pkb);
			list<string> expectedResults = {	"2 11 1", "2 33 1", "2 77 1",
												"1 11 3", "1 33 3", "1 77 3",
												"2 11 7", "2 33 7", "2 77 7" };

			Assert::IsTrue(isSameResult(expectedResults, actualResults));
		}
		TEST_METHOD(selectTupleDiffTable3) {
			ResultProjector resultProjector;
			// set up
			static unordered_map<string, int> synonymTable;
			static unordered_map<int, list<unordered_map<string, int>>> synonymResults;

			synonymTable["a"] = 0;
			synonymTable["b"] = 0;
			synonymTable["c"] = 1;
			synonymTable["d"] = 1;
			synonymTable["e"] = 2;

			list<unordered_map<string, int>> synonymResult1;
			synonymResult1.push_back({ {"a", 1}, {"b", 2} });
			synonymResult1.push_back({ {"a", 3}, {"b", 1} });
			synonymResult1.push_back({ {"a", 7}, {"b", 2} });
			synonymResults[0] = synonymResult1;
			list<unordered_map<string, int>> synonymResult2;
			synonymResult2.push_back({ {"c", 11}, {"d", 22} });
			synonymResult2.push_back({ {"c", 33}, {"d", 11} });
			synonymResult2.push_back({ {"c", 77}, {"d", 22} });
			synonymResults[1] = synonymResult2;
			list<unordered_map<string, int>> synonymResult3;
			synonymResult3.push_back({ {"e", 4} });
			synonymResult3.push_back({ {"e", 6} });
			synonymResults[2] = synonymResult3;

			resultProjector.setSynonymTable(synonymTable);
			resultProjector.setSynonymResults(synonymResults);

			vector<DesignEntity> selectedSynonyms = {	DesignEntity("b", Type::STATEMENT),
														DesignEntity("e", Type::ASSIGN),
														DesignEntity("c", Type::ASSIGN) };
			PKB pkb; // dummy

			// get results
			list<string> actualResults = resultProjector.getResults(selectedSynonyms, pkb);
			list<string> expectedResults = {	"2 4 11", "2 6 11", "2 4 33", "2 6 33", "2 4 77", "2 6 77",
												"1 4 11", "1 6 11", "1 4 33", "1 6 33", "1 4 77", "1 6 77",
												"2 4 11", "2 6 11", "2 4 33", "2 6 33", "2 4 77", "2 6 77" };

			Assert::IsTrue(isSameResult(expectedResults, actualResults));
		}

		// ---------------------------------- HELPER FUNCTIONS -----------------------------------------------
		// order in list does not matter
		bool isSameResult(list<string> expectedResults, list<string> actualResults) {
			if (expectedResults.size() != actualResults.size()) {
				return false;
			}

			unordered_set<string> expectedResultsSet;
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
