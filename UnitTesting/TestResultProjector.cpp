#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ResultProjector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestResultProjector)
	{
	public:
		/* Convention:
			OneOne -> 1 synonym from first clause, 1 synonym from second clause
			OneTwo -> 1 synonym from first clause, 2 synonyms from second clause
			TwoOne -> 2 synonyms from first clause, 1 synonym from second clause
			TwoTwo -> 2 synonyms from first clause, 2 synonyms from second clause */

		// OneOne
		TEST_METHOD(combineOneOneWithNoCommonSynonym)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result2["b"] = { 4,5,3 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 1;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 9,3,5,2 };
			expectedSynonymResults[0] = expectedResults1;
			unordered_map<string, list<int>> expectedResults2;
			expectedResults2["b"] = { 4,5,3 };
			expectedSynonymResults[1] = expectedResults2;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneOneWithOneCommonSynonym)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result2["a"] = { 4,5,3 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults;
			expectedResults["a"] = { 3,5 };
			expectedSynonymResults[0] = expectedResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneOneWithOneCommonSynonymWithNoCommonResults)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result2["a"] = { 4 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameTable);
		}

		// OneTwo
		TEST_METHOD(combineOneTwoWithNoCommonSynonym)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result2["b"] = { 1,2,3,4 };
			result2["c"] = { 4,5,3 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 1;
			expectedSynonymTable["c"] = 1;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 9,3,5,2 };
			expectedSynonymResults[0] = expectedResults1;
			unordered_map<string, list<int>> expectedResults2;
			expectedResults2["b"] = { 1,2,3,4 };
			expectedResults2["c"] = { 4,5,3 };
			expectedSynonymResults[1] = expectedResults2;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneTwoWithOneCommonSynonym)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result2["b"] = { 1,2,3 };
			result2["a"] = { 4,5,3 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 3,5 };
			expectedResults1["b"] = { 3,2 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneTwoWithOneCommonSynonym2)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["b"] = { 2,5,6 };
			result2["a"] = { 1,2,4,5,7 };
			result2["b"] = { 2,5,1,2,8 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 1,2,5 };
			expectedResults1["b"] = { 2,5,2 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneTwoWithOneCommonSynonym3)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 1,2,4,4,4,8,2 };
			result2["a"] = { 1,2,3,4,5,7 };
			result2["b"] = { 2,5,6,1,2,8 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 1,2,4,4,4,2 };
			expectedResults1["b"] = { 2,5,1,1,1,5 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneTwoWithOneCommonSynonymWithNoCommonResults)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result2["a"] = { 1,8,6,4 };
			result2["b"] = { 4,6,2,1 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		// TwoOne
		TEST_METHOD(combineTwoOneWithNoCommonSynonym)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result1["b"] = { 1,2,3,4 };
			result2["c"] = { 4,5,3 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 1;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 9,3,5,2 };
			expectedResults1["b"] = { 1,2,3,4 };
			expectedSynonymResults[0] = expectedResults1;
			unordered_map<string, list<int>> expectedResults2;
			expectedResults2["c"] = { 4,5,3 };
			expectedSynonymResults[1] = expectedResults2;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoOneWithOneCommonSynonym)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result1["b"] = { 1,5,3,4 };
			result2["b"] = { 3,1 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 9,5 };
			expectedResults1["b"] = { 1,3 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoOneWithOneCommonSynonymWithNoCommonResults)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result1["b"] = { 1,5,3,4 };
			result2["b"] = { 2,6 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		// TwoTwo
		TEST_METHOD(combineTwoTwoWithNoCommonSynonym)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result1["b"] = { 1,2,3,4 };
			result2["c"] = { 4,5,3 };
			result2["d"] = { 2,5,3 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 1;
			expectedSynonymTable["d"] = 1;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 9,3,5,2 };
			expectedResults1["b"] = { 1,2,3,4 };
			expectedSynonymResults[0] = expectedResults1;
			unordered_map<string, list<int>> expectedResults2;
			expectedResults2["c"] = { 4,5,3 };
			expectedResults2["d"] = { 2,5,3 };
			expectedSynonymResults[1] = expectedResults2;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithOneCommonSynonym)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result1["b"] = { 1,5,3,4 };
			result2["b"] = { 5,2,1 };
			result2["c"] = { 6,9,2 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 9,3 };
			expectedResults1["b"] = { 1,5 };
			expectedResults1["c"] = { 2,6 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithOneCommonSynonym2)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result1["b"] = { 1,5,3,4 };
			result2["a"] = { 5,2,1,1,4,2,2,5 };
			result2["c"] = { 6,9,2,3,2,1,4,7 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 5,2,2,2,5 };
			expectedResults1["b"] = { 3,4,4,4,3 };
			expectedResults1["c"] = { 6,9,1,4,7 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithOneCommonSynonym3)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result1["b"] = { 1,5,3,4 };
			result2["b"] = { 5,2,1,1,4,5,9,5 };
			result2["c"] = { 6,9,2,3,2,7,2,1 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 9,3,2,9,3,3 };
			expectedResults1["b"] = { 1,5,4,1,5,5 };
			expectedResults1["c"] = { 2,6,2,3,7,1 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithOneCommonSynonymWithAllCommonResults)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result1["b"] = { 1,5,3,4 };
			result2["b"] = { 1,5,3,4 };
			result2["c"] = { 2,7,6,1 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 9,3,5,2 };
			expectedResults1["b"] = { 1,5,3,4 };
			expectedResults1["c"] = { 2,7,6,1 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithOneCommonSynonymWithAllCommonResults2)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5 };
			result1["b"] = { 1,3,4 };
			result2["b"] = { 1,1,3,3,4,4 };
			result2["c"] = { 2,3,4,5,6,7 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			expectedSynonymTable["c"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 9,3,5,9,3,5 };
			expectedResults1["b"] = { 1,3,4,1,3,4 };
			expectedResults1["c"] = { 2,4,6,3,5,7 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithOneCommonSynonymWithNoCommonResults)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result1["b"] = { 1,5,3,4 };
			result2["a"] = { 1,4 };
			result2["c"] = { 2,6 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		TEST_METHOD(combineTwoTwoWithTwoCommonSynonym)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,2 };
			result1["b"] = { 1,5,3,4 };
			result2["a"] = { 3,2,7,3 };
			result2["b"] = { 2,4,1,5 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 3,2 };
			expectedResults1["b"] = { 5,4 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithTwoCommonSynonym2)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 1,2,2,4 };
			result1["b"] = { 1,3,4,5 };
			result2["a"] = { 2,2,3,4,4,5 };
			result2["b"] = { 5,3,1,5,6,1 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 2,4 };
			expectedResults1["b"] = { 3,5 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithTwoCommonSynonym3)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 2,2,3,4,4,5 };
			result1["b"] = { 5,3,1,5,6,1 };
			result2["a"] = { 1,4,2,2 };
			result2["b"] = { 1,5,4,3 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 2,4 };
			expectedResults1["b"] = { 3,5 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithTwoCommonSynonymWithAllCommonResults)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 1,2,2,4 };
			result1["b"] = { 1,3,4,5 };
			result2["a"] = { 2,1,4,2 };
			result2["b"] = { 4,1,5,3 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 1,2,2,4 };
			expectedResults1["b"] = { 1,3,4,5 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithTwoCommonSynonymWithNoCommonResults)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 1,2,2,4 };
			result1["b"] = { 1,3,4,5 };
			result2["a"] = { 2,1,4,2 };
			result2["b"] = { 1,2,4,2 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineTwoTwoWithTwoCommonSynonymWithNoCommonResults2)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			result1["a"] = { 9,3,5,1,2,2 };
			result1["b"] = { 1,3,5,2,5,1 };
			result2["a"] = { 1,2,2,4 };
			result2["b"] = { 1,3,4,5 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}

		// testing merging different tables
		TEST_METHOD(combineOneOneTwoWithTwoCommonSynonym)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			unordered_map<string, list<int>> result3;
			result1["b"] = { 2,5,6 };
			result2["a"] = { 1,2,4,5,6,7,2 };
			result3["a"] = { 1,2,3,4,5,7 };
			result3["b"] = { 2,5,6,1,2,8 };


			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);
			resultProjector.combineResults(result3);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 0;
			expectedSynonymTable["b"] = 0;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 1,2,5 };
			expectedResults1["b"] = { 2,5,2 };
			expectedSynonymResults[0] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
		TEST_METHOD(combineOneOneTwoWithTwoCommonSynonym2)
		{
			ResultProjector resultProjector;
			// set up
			resultProjector.resetResults();

			unordered_map<string, list<int>> result1;
			unordered_map<string, list<int>> result2;
			unordered_map<string, list<int>> result3;
			result1["b"] = { 2,5,6 };
			result2["a"] = { 1,2,4,4,4,8,2 };
			result2["c"] = { 2,3,7,4,1,6,9 };
			result3["a"] = { 1,2,3,4,5,7 };
			result3["b"] = { 2,5,6,1,2,8 };

			// combining results
			resultProjector.combineResults(result1);
			resultProjector.combineResults(result2);
			resultProjector.combineResults(result3);

			// expected results
			unordered_map<string, int> expectedSynonymTable;
			expectedSynonymTable["a"] = 1;
			expectedSynonymTable["b"] = 1;
			expectedSynonymTable["c"] = 1;
			unordered_map<int, unordered_map<string, list<int>>> expectedSynonymResults;
			unordered_map<string, list<int>> expectedResults1;
			expectedResults1["a"] = { 1,2,2 };
			expectedResults1["b"] = { 2,5,5 };
			expectedResults1["c"] = { 2,3,9 };
			expectedSynonymResults[1] = expectedResults1;

			// compare results
			bool sameTable = expectedSynonymTable == resultProjector.getSynonymTable();
			bool sameResult = expectedSynonymResults == resultProjector.getSynonymResults();
			Assert::IsTrue(sameTable);
			Assert::IsTrue(sameResult);
		}
	};
}
