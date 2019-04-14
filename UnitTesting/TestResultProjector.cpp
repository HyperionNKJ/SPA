#include "stdafx.h"
#include "CppUnitTest.h"
#include "ResultProjector.h"
#include "NextT.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	ResultProjector resultProjector;
	static std::unordered_map<std::string, int> synonymTable;
	static std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> synonymResults;

	TEST_CLASS(TestResultProjector) {
	
	public:
		TEST_METHOD_INITIALIZE(setup)
		{
			resultProjector.resetResults();
		}

		/* Convention:
			OneOne -> 1 synonym from first clause, 1 synonym from second clause
			OneTwo -> 1 synonym from first clause, 2 synonyms from second clause
			TwoOne -> 2 synonyms from first clause, 1 synonym from second clause
			TwoTwo -> 2 synonyms from first clause, 2 synonyms from second clause */

		// OneOne
		TEST_METHOD(combineOneOneWithNoCommonSynonym) {

			std::vector<std::string> result1Synonym { "a" };
			std::vector<std::string> result2Synonym { "b" };

			std::unordered_set<int> result1;
			std::unordered_set<int> result2;
			result1 = { 9,3,5,2 };
			result2 = { 4,5,3 };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 1;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults1;
			expectedResults1.push_back({ {"a", 9} });
			expectedResults1.push_back({ {"a", 3} });
			expectedResults1.push_back({ {"a", 5} });
			expectedResults1.push_back({ {"a", 2} });
			expectedSynonymResults[0] = expectedResults1;

			std::list<std::unordered_map<std::string, int>> expectedResults2;
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

			std::vector<std::string> result1Synonym{ "a" };
			std::vector<std::string> result2Synonym{ "a" };

			std::unordered_set<int> result1;
			std::unordered_set<int> result2;
			result1 = { 9,3,5,2 };
			result2 = { 4,5,3 };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;


			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a" };
			std::vector<std::string> result2Synonym{ "a" };

			std::unordered_set<int> result1;
			std::unordered_set<int> result2;
			result1 = { 9,3,5,2 };
			result2 = { 4 };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			std::unordered_map<std::string, int> expectedSynonymTable;
			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		// OneTwo
		TEST_METHOD(combineOneTwoWithNoCommonSynonym) {

			std::vector<std::string> result1Synonym{ "a" };
			std::vector<std::string> result2Synonym{ "b", "c" };

			std::unordered_set<int> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
			result1 = { 9,3,5,2 };
			result2 = {	{1, {4,6}},
						{2, {5}},
						{3, {3}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 1;
			expectedSynonymTable["c"] = 1;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults1;
			expectedResults1.push_back({ {"a", 9} });
			expectedResults1.push_back({ {"a", 3} });
			expectedResults1.push_back({ {"a", 5} });
			expectedResults1.push_back({ {"a", 2} });
			expectedSynonymResults[0] = expectedResults1;

			std::list<std::unordered_map<std::string, int>> expectedResults2;
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

			std::vector<std::string> result1Synonym{ "a" };
			std::vector<std::string> result2Synonym{ "b", "a" };

			std::unordered_set<int> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
			result1 = { 9,3,5,2 };
			result2 = { {1, {4}},
						{2, {5,3}},
						{3, {3}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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
			
			std::vector<std::string> result1Synonym{ "b" };
			std::vector<std::string> result2Synonym{ "b", "a" };

			std::unordered_set<int> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
			result1 = { 2,5,6 };
			result2 = { {2, {1,5}},
						{5, {2}},
						{1, {4}},
						{8, {7}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a" };
			std::vector<std::string> result2Synonym{ "a", "b" };

			std::unordered_set<int> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a" };
			std::vector<std::string> result2Synonym{ "a", "b" };

			std::unordered_set<int> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
			result1 = { 9,3,5,2 };
			result2 = { {1, {4}},
						{8, {6}},
						{6, {2}},
						{4, {1}} };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			std::unordered_map<std::string, int> expectedSynonymTable;
			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		// TwoOne
		TEST_METHOD(combineTwoOneWithNoCommonSynonym) {

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "c" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_set<int> result2;
			result1 = { {9, {1}},
						{3, {2}},
						{5, {3}},
						{2, {4}} };
			result2 = { 4,5,3 };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 1;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults1;
			expectedResults1.push_back({ {"a", 9}, {"b", 1} });
			expectedResults1.push_back({ {"a", 3}, {"b", 2} });
			expectedResults1.push_back({ {"a", 5}, {"b", 3} });
			expectedResults1.push_back({ {"a", 2}, {"b", 4} });
			expectedSynonymResults[0] = expectedResults1;

			std::list<std::unordered_map<std::string, int>> expectedResults2;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "b" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_set<int> result2;
			result1 = { {9, {1}},
						{3, {2}},
						{5, {3}},
						{2, {4}} };
			result2 = { 3,1 };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "b" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_set<int> result2;
			result1 = { {9, {1}},
						{3, {5}},
						{5, {3}},
						{2, {4}} };
			result2 = { 2,6 };

			// combining results
			resultProjector.combineResults(result1, result1Synonym);
			resultProjector.combineResults(result2, result2Synonym);

			// expected results
			std::unordered_map<std::string, int> expectedSynonymTable;
			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		// TwoTwo
		TEST_METHOD(combineTwoTwoWithNoCommonSynonym) {

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "c", "d" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 1;
			expectedSynonymTable["d"] = 1;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults1;
			expectedResults1.push_back({ {"a", 9}, {"b", 1} });
			expectedResults1.push_back({ {"a", 3}, {"b", 2} });
			expectedResults1.push_back({ {"a", 5}, {"b", 3} });
			expectedResults1.push_back({ {"a", 2}, {"b", 4} });
			expectedSynonymResults[0] = expectedResults1;

			std::list<std::unordered_map<std::string, int>> expectedResults2;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "b", "c" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "a", "c" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "b", "c" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "b", "c" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "c", "b" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "b", "c" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "a", "c" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		TEST_METHOD(combineTwoTwoWithTwoCommonSynonym) {

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "b", "a" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "a", "b" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "a", "b" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "a", "b" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "a", "b" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithTwoCommonSynonymWithNoCommonResults2) {

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "a", "b" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoTwoWithOneCommonSynonymSameTable) {

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "b", "c" };
			std::vector<std::string> result3Synonym{ "c", "d" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
			std::unordered_map<int, std::unordered_set<int>> result3;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;
			expectedSynonymTable["d"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "c", "b" };
			std::vector<std::string> result3Synonym{ "c", "d" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
			std::unordered_map<int, std::unordered_set<int>> result3;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;
			expectedSynonymTable["d"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "a", "b" };
			std::vector<std::string> result2Synonym{ "a", "c" };
			std::vector<std::string> result3Synonym{ "b", "d" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
			std::unordered_map<int, std::unordered_set<int>> result3;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;
			expectedSynonymTable["d"] = 0;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "b" };
			std::vector<std::string> result2Synonym{ "a" };
			std::vector<std::string> result3Synonym{ "a", "b" };

			std::unordered_set<int> result1;
			std::unordered_set<int> result2;
			std::unordered_map<int, std::unordered_set<int>> result3;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 2;
			expectedSynonymTable["b"] = 2;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "b" };
			std::vector<std::string> result2Synonym{ "a", "c" };
			std::vector<std::string> result3Synonym{ "a", "b" };

			std::unordered_set<int> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
			std::unordered_map<int, std::unordered_set<int>> result3;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 2;
			expectedSynonymTable["b"] = 2;
			expectedSynonymTable["c"] = 2;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

			std::vector<std::string> result1Synonym{ "b", "d" };
			std::vector<std::string> result2Synonym{ "a", "c" };
			std::vector<std::string> result3Synonym{ "a", "b" };

			std::unordered_map<int, std::unordered_set<int>> result1;
			std::unordered_map<int, std::unordered_set<int>> result2;
			std::unordered_map<int, std::unordered_set<int>> result3;
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
			std::unordered_map<std::string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 2;
			expectedSynonymTable["b"] = 2;
			expectedSynonymTable["c"] = 2;
			expectedSynonymTable["d"] = 2;

			std::unordered_map<int, std::list<std::unordered_map<std::string, int>>> expectedSynonymResults;
			std::list<std::unordered_map<std::string, int>> expectedResults;
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

		// ---------------------------------- TEST SELECT -----------------------------------------------
	
		TEST_METHOD(selectOneSynonym) {

			synonymTable["a"] = 0;
			synonymTable["b"] = 0;
			synonymTable["c"] = 0;

			std::list<std::unordered_map<std::string, int>> synonymResult;
			synonymResult.push_back({ {"a", 1}, {"b", 2}, {"c", 2} });
			synonymResult.push_back({ {"a", 3}, {"b", 1}, {"c", 1} });
			synonymResult.push_back({ {"a", 7}, {"b", 2}, {"c", 9} });
			synonymResult.push_back({ {"a", 2}, {"b", 4}, {"c", 5} });
			synonymResult.push_back({ {"a", 5}, {"b", 5}, {"c", 7} });
			synonymResults[0] = synonymResult;

			resultProjector.setSynonymTable(synonymTable);
			resultProjector.setSynonymResults(synonymResults);

			std::vector<DesignEntity> selectedSynonyms = { DesignEntity("a", Type::STATEMENT) };
			PKB pkb; // dummy

			// get results
			std::list<std::string> actualResults = resultProjector.getResults(selectedSynonyms, pkb);
			std::list<std::string> expectedResults = { "1", "3", "7", "2", "5" };

			Assert::IsTrue(isSameResult(expectedResults, actualResults));
		}
		TEST_METHOD(selectTupleSameTable) {

			synonymTable["a"] = 0;
			synonymTable["b"] = 0;
			synonymTable["c"] = 0;

			std::list<std::unordered_map<std::string, int>> synonymResult;
			synonymResult.push_back({ {"a", 1}, {"b", 2}, {"c", 2} });
			synonymResult.push_back({ {"a", 3}, {"b", 1}, {"c", 1} });
			synonymResult.push_back({ {"a", 7}, {"b", 2}, {"c", 9} });
			synonymResult.push_back({ {"a", 2}, {"b", 4}, {"c", 5} });
			synonymResult.push_back({ {"a", 5}, {"b", 5}, {"c", 7} });
			synonymResults[0] = synonymResult;

			resultProjector.setSynonymTable(synonymTable);
			resultProjector.setSynonymResults(synonymResults);

			std::vector<DesignEntity> selectedSynonyms = {	DesignEntity("b", Type::STATEMENT),
														DesignEntity("a", Type::ASSIGN) };
			PKB pkb; // dummy
			
			// get results
			std::list<std::string> actualResults = resultProjector.getResults(selectedSynonyms, pkb);
			std::list<std::string> expectedResults = { "2 1", "1 3", "2 7", "4 2", "5 5" };

			Assert::IsTrue(isSameResult(expectedResults, actualResults));
		}
		TEST_METHOD(selectTupleDiffTable) {

			synonymTable["a"] = 0;
			synonymTable["b"] = 0;
			synonymTable["c"] = 1;
			synonymTable["d"] = 1;

			std::list<std::unordered_map<std::string, int>> synonymResult1;
			synonymResult1.push_back({ {"a", 1}, {"b", 2} });
			synonymResult1.push_back({ {"a", 3}, {"b", 1} });
			synonymResult1.push_back({ {"a", 7}, {"b", 2} });
			synonymResults[0] = synonymResult1;
			std::list<std::unordered_map<std::string, int>> synonymResult2;
			synonymResult2.push_back({ {"c", 11}, {"d", 22} });
			synonymResult2.push_back({ {"c", 33}, {"d", 11} });
			synonymResult2.push_back({ {"c", 77}, {"d", 22} });
			synonymResults[1] = synonymResult2;

			resultProjector.setSynonymTable(synonymTable);
			resultProjector.setSynonymResults(synonymResults);

			std::vector<DesignEntity> selectedSynonyms = {	DesignEntity("b", Type::STATEMENT),
														DesignEntity("c", Type::ASSIGN) };
			PKB pkb; // dummy

			// get results
			std::list<std::string> actualResults = resultProjector.getResults(selectedSynonyms, pkb);
			std::list<std::string> expectedResults = {	"2 11", "2 33", "2 77",
												"1 11", "1 33", "1 77" };

			Assert::IsTrue(isSameResult(expectedResults, actualResults));
		}
		TEST_METHOD(selectTupleDiffTable2) {

			synonymTable["a"] = 0;
			synonymTable["b"] = 0;
			synonymTable["c"] = 1;
			synonymTable["d"] = 1;

			std::list<std::unordered_map<std::string, int>> synonymResult1;
			synonymResult1.push_back({ {"a", 1}, {"b", 2} });
			synonymResult1.push_back({ {"a", 3}, {"b", 1} });
			synonymResult1.push_back({ {"a", 7}, {"b", 2} });
			synonymResults[0] = synonymResult1;
			std::list<std::unordered_map<std::string, int>> synonymResult2;
			synonymResult2.push_back({ {"c", 11}, {"d", 22} });
			synonymResult2.push_back({ {"c", 33}, {"d", 11} });
			synonymResult2.push_back({ {"c", 77}, {"d", 22} });
			synonymResults[1] = synonymResult2;

			resultProjector.setSynonymTable(synonymTable);
			resultProjector.setSynonymResults(synonymResults);

			std::vector<DesignEntity> selectedSynonyms = {	DesignEntity("b", Type::STATEMENT),
														DesignEntity("c", Type::ASSIGN),
														DesignEntity("a", Type::ASSIGN) };
			PKB pkb; // dummy

			// get results
			std::list<std::string> actualResults = resultProjector.getResults(selectedSynonyms, pkb);
			std::list<std::string> expectedResults = {	"2 11 1", "2 33 1", "2 77 1",
												"1 11 3", "1 33 3", "1 77 3",
												"2 11 7", "2 33 7", "2 77 7" };

			Assert::IsTrue(isSameResult(expectedResults, actualResults));
		}
		TEST_METHOD(selectTupleDiffTable3) {

			synonymTable["a"] = 0;
			synonymTable["b"] = 0;
			synonymTable["c"] = 1;
			synonymTable["d"] = 1;
			synonymTable["e"] = 2;

			std::list<std::unordered_map<std::string, int>> synonymResult1;
			synonymResult1.push_back({ {"a", 1}, {"b", 2} });
			synonymResult1.push_back({ {"a", 3}, {"b", 1} });
			synonymResult1.push_back({ {"a", 7}, {"b", 2} });
			synonymResults[0] = synonymResult1;
			std::list<std::unordered_map<std::string, int>> synonymResult2;
			synonymResult2.push_back({ {"c", 11}, {"d", 22} });
			synonymResult2.push_back({ {"c", 33}, {"d", 11} });
			synonymResult2.push_back({ {"c", 77}, {"d", 22} });
			synonymResults[1] = synonymResult2;
			std::list<std::unordered_map<std::string, int>> synonymResult3;
			synonymResult3.push_back({ {"e", 4} });
			synonymResult3.push_back({ {"e", 6} });
			synonymResults[2] = synonymResult3;

			resultProjector.setSynonymTable(synonymTable);
			resultProjector.setSynonymResults(synonymResults);

			std::vector<DesignEntity> selectedSynonyms = {	DesignEntity("b", Type::STATEMENT),
														DesignEntity("e", Type::ASSIGN),
														DesignEntity("c", Type::ASSIGN) };
			PKB pkb; // dummy

			// get results
			std::list<std::string> actualResults = resultProjector.getResults(selectedSynonyms, pkb);
			std::list<std::string> expectedResults = {	"2 4 11", "2 6 11", "2 4 33", "2 6 33", "2 4 77", "2 6 77",
												"1 4 11", "1 6 11", "1 4 33", "1 6 33", "1 4 77", "1 6 77" };

			Assert::IsTrue(isSameResult(expectedResults, actualResults));
		}

		// ---------------------------------- TEST CACHING -----------------------------------------------
		TEST_METHOD(cacheSynSynSame) {

			DesignEntity paramOne = DesignEntity("s", Type::STATEMENT, AttrRef::UNASSIGNED);
			DesignEntity paramTwo = DesignEntity("s", Type::STATEMENT, AttrRef::UNASSIGNED);
			NextT* clause = new NextT(paramOne, paramTwo);

			std::unordered_map<int, std::unordered_set<int>> result;
			result = { {2, {1,3}},
						{5, {4,6}},
						{6, {2,8,3}} };

			resultProjector.storeInCache(clause, result);

			ResultCache expectedCache;
			std::unordered_map<CacheType, std::unordered_map<int, std::unordered_set<int>>> expectedResult;
			expectedResult[CacheType::STMT_STMT] = result;
			expectedCache.setSynSynSame(expectedResult);
			Assert::IsTrue(resultProjector.getNextTCache().isEquals(expectedCache));

			paramOne = DesignEntity("s1", Type::STATEMENT, AttrRef::UNASSIGNED);
			paramTwo = DesignEntity("s1", Type::STATEMENT, AttrRef::UNASSIGNED);
			clause = new NextT(paramOne, paramTwo);
			Assert::IsTrue(resultProjector.cacheExists(clause));

			bool isSameResultCached = resultProjector.getNextTCache().getTwoSynCacheResult() == result;
			Assert::IsTrue(isSameResultCached);
		}
		TEST_METHOD(cacheSynSynDiff) {

			DesignEntity paramOne = DesignEntity("r", Type::READ, AttrRef::UNASSIGNED);
			DesignEntity paramTwo = DesignEntity("s", Type::STATEMENT, AttrRef::UNASSIGNED);
			NextT* clause = new NextT(paramOne, paramTwo);

			std::unordered_map<int, std::unordered_set<int>> result;
			result = { {2, {1,3}},
						{5, {4,6}},
						{6, {2,8,3}} };

			resultProjector.storeInCache(clause, result);

			ResultCache expectedCache;
			std::unordered_map<CacheType, std::unordered_map<int, std::unordered_set<int>>> expectedResult;
			expectedResult[CacheType::READ_STMT] = result;
			expectedCache.setSynSynDiff(expectedResult);
			Assert::IsTrue(resultProjector.getNextTCache().isEquals(expectedCache));

			paramOne = DesignEntity("r1", Type::READ, AttrRef::UNASSIGNED);
			paramTwo = DesignEntity("s1", Type::STATEMENT, AttrRef::UNASSIGNED);
			clause = new NextT(paramOne, paramTwo);
			Assert::IsTrue(resultProjector.cacheExists(clause));

			bool isSameResultCached = resultProjector.getNextTCache().getTwoSynCacheResult() == result;
			Assert::IsTrue(isSameResultCached);

			paramOne = DesignEntity("r", Type::READ, AttrRef::UNASSIGNED);
			paramTwo = DesignEntity("a", Type::ASSIGN, AttrRef::UNASSIGNED);
			clause = new NextT(paramOne, paramTwo);
			Assert::IsFalse(resultProjector.cacheExists(clause));
		}
		TEST_METHOD(cacheSynSynDiff2) {

			DesignEntity paramOne = DesignEntity("s", Type::STATEMENT, AttrRef::UNASSIGNED);
			DesignEntity paramTwo = DesignEntity("s1", Type::STATEMENT, AttrRef::UNASSIGNED);
			NextT* clause = new NextT(paramOne, paramTwo);

			std::unordered_map<int, std::unordered_set<int>> result;
			result = { {2, {1,3}},
						{5, {4,6}},
						{6, {2,8,3}} };

			resultProjector.storeInCache(clause, result);

			ResultCache expectedCache;
			std::unordered_map<CacheType, std::unordered_map<int, std::unordered_set<int>>> expectedResult;
			expectedResult[CacheType::STMT_STMT] = result;
			expectedCache.setSynSynDiff(expectedResult);
			Assert::IsTrue(resultProjector.getNextTCache().isEquals(expectedCache));

			paramOne = DesignEntity("s2", Type::STATEMENT, AttrRef::UNASSIGNED);
			paramTwo = DesignEntity("s3", Type::STATEMENT, AttrRef::UNASSIGNED);
			clause = new NextT(paramOne, paramTwo);
			Assert::IsTrue(resultProjector.cacheExists(clause));

			bool isSameResultCached = resultProjector.getNextTCache().getTwoSynCacheResult() == result;
			Assert::IsTrue(isSameResultCached);
		}
		TEST_METHOD(cacheSynUnderscore) {

			DesignEntity paramOne = DesignEntity("s", Type::STATEMENT, AttrRef::UNASSIGNED);
			DesignEntity paramTwo = DesignEntity("_", Type::UNDERSCORE, AttrRef::UNASSIGNED);
			NextT* clause = new NextT(paramOne, paramTwo);

			std::unordered_set<int> result;
			result = { 2,1,5,8 };

			resultProjector.storeInCache(clause, result);

			ResultCache expectedCache;
			std::unordered_map<Type, std::unordered_set<int>> expectedResult;
			expectedResult[Type::STATEMENT] = result;
			expectedCache.setSynUnderscore(expectedResult);
			Assert::IsTrue(resultProjector.getNextTCache().isEquals(expectedCache));

			paramOne = DesignEntity("s1", Type::STATEMENT, AttrRef::UNASSIGNED);
			paramTwo = DesignEntity("_", Type::UNDERSCORE, AttrRef::UNASSIGNED);
			clause = new NextT(paramOne, paramTwo);
			Assert::IsTrue(resultProjector.cacheExists(clause));

			bool isSameResultCached = resultProjector.getNextTCache().getOneSynCacheResult() == result;
			Assert::IsTrue(isSameResultCached);
		}
		TEST_METHOD(cacheUnderscoreSyn) {

			DesignEntity paramOne = DesignEntity("_", Type::UNDERSCORE, AttrRef::UNASSIGNED);
			DesignEntity paramTwo = DesignEntity("pn", Type::PRINT, AttrRef::UNASSIGNED);
			NextT* clause = new NextT(paramOne, paramTwo);

			std::unordered_set<int> result;
			result = { 2,1,5,8 };

			resultProjector.storeInCache(clause, result);

			ResultCache expectedCache;
			std::unordered_map<Type, std::unordered_set<int>> expectedResult;
			expectedResult[Type::PRINT] = result;
			expectedCache.setUnderscoreSyn(expectedResult);
			Assert::IsTrue(resultProjector.getNextTCache().isEquals(expectedCache));

			paramOne = DesignEntity("_", Type::UNDERSCORE, AttrRef::UNASSIGNED);
			paramTwo = DesignEntity("pn1", Type::PRINT, AttrRef::UNASSIGNED);
			clause = new NextT(paramOne, paramTwo);
			Assert::IsTrue(resultProjector.cacheExists(clause));

			bool isSameResultCached = resultProjector.getNextTCache().getOneSynCacheResult() == result;
			Assert::IsTrue(isSameResultCached);
		}
		TEST_METHOD(cacheSynFixed) {

			DesignEntity paramOne = DesignEntity("cl", Type::CALL, AttrRef::UNASSIGNED);
			DesignEntity paramTwo = DesignEntity("2", Type::FIXED, AttrRef::UNASSIGNED);
			NextT* clause = new NextT(paramOne, paramTwo);

			std::unordered_set<int> result;
			result = { 2,1,5,8 };

			resultProjector.storeInCache(clause, result);

			ResultCache expectedCache;
			std::unordered_map<Type, std::unordered_map<int, std::unordered_set<int>>> expectedResult;
			expectedResult[Type::CALL][2] = result;
			expectedCache.setSynFixed(expectedResult);
			Assert::IsTrue(resultProjector.getNextTCache().isEquals(expectedCache));

			paramOne = DesignEntity("cl2", Type::CALL, AttrRef::UNASSIGNED);
			paramTwo = DesignEntity("2", Type::FIXED, AttrRef::UNASSIGNED);
			clause = new NextT(paramOne, paramTwo);
			Assert::IsTrue(resultProjector.cacheExists(clause));

			bool isSameResultCached = resultProjector.getNextTCache().getOneSynCacheResult() == result;
			Assert::IsTrue(isSameResultCached);

			paramOne = DesignEntity("cl2", Type::CALL, AttrRef::UNASSIGNED);
			paramTwo = DesignEntity("3", Type::FIXED, AttrRef::UNASSIGNED);
			clause = new NextT(paramOne, paramTwo);
			Assert::IsFalse(resultProjector.cacheExists(clause));
		}
		TEST_METHOD(cacheFixedSyn) {

			DesignEntity paramOne = DesignEntity("1", Type::FIXED, AttrRef::UNASSIGNED);
			DesignEntity paramTwo = DesignEntity("w", Type::WHILE, AttrRef::UNASSIGNED);
			NextT* clause = new NextT(paramOne, paramTwo);

			std::unordered_set<int> result;
			result = { 2,1,5,8 };

			resultProjector.storeInCache(clause, result);

			ResultCache expectedCache;
			std::unordered_map<Type, std::unordered_map<int, std::unordered_set<int>>> expectedResult;
			expectedResult[Type::WHILE][1] = result;
			expectedCache.setFixedSyn(expectedResult);
			Assert::IsTrue(resultProjector.getNextTCache().isEquals(expectedCache));

			paramOne = DesignEntity("1", Type::FIXED, AttrRef::UNASSIGNED);
			paramTwo = DesignEntity("w2", Type::WHILE, AttrRef::UNASSIGNED);
			clause = new NextT(paramOne, paramTwo);
			Assert::IsTrue(resultProjector.cacheExists(clause));

			bool isSameResultCached = resultProjector.getNextTCache().getOneSynCacheResult() == result;
			Assert::IsTrue(isSameResultCached);

			paramOne = DesignEntity("3", Type::FIXED, AttrRef::UNASSIGNED);
			paramTwo = DesignEntity("w3", Type::WHILE, AttrRef::UNASSIGNED);
			clause = new NextT(paramOne, paramTwo);
			Assert::IsFalse(resultProjector.cacheExists(clause));
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
