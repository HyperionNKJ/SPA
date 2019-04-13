#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestParser) {
	public:
		TEST_METHOD(TestHandleProcedure) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			int result;
			string expectedProc = "myProcOne";

			result = parser.handleProcedure("procedure myProcOne {");
			Assert::AreEqual(result, 0, L"incorrect", LINE_INFO());
			Assert::AreEqual(0, expectedProc.compare(parser.getCurrentProcedure()), L"incorrect", LINE_INFO());

			parser.setWithinProcedure(false);

			expectedProc = "if";
			result = parser.handleProcedure("  procedure   if{");
			cout << result << endl;
			Assert::AreEqual(result, 0, L"incorrect", LINE_INFO());
			Assert::AreEqual(0, expectedProc.compare(parser.getCurrentProcedure()), L"incorrect", LINE_INFO());

			unordered_set<string>expectedProcSet = {"myProcOne", "if"};
			bool isEqualExpectedProcSet = expectedProcSet == parser.getProcNames();
			Assert::IsTrue(isEqualExpectedProcSet, L"incorrect", LINE_INFO());

			result = parser.handleProcedure("  procedure myProcOne{");
			Assert::AreEqual(result, -1, L"incorrect", LINE_INFO());

			result = parser.handleProcedure("  procedure { ");
			Assert::AreEqual(result, -1, L"incorrect", LINE_INFO());

		}

		/*Tests to handle statements in the code must ensure the following update correctly
		Or are not updated, depending on the statement type.
		Parent Vector, Follow Vector, All Follow Stack, statement number
		Also ensure correct return integer to signal success or failure
		*/
		TEST_METHOD(TestHandleAssignment) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			int result;
			
			parser.setStatementNumber(7);
			vector<int> testFollowVector = { 5, 6 };
			vector<int> expectedFollowVector = vector<int>(testFollowVector);
			expectedFollowVector.push_back(7);
			vector<int> expectedParentVector = { 1, 2 };

			vector<int> tempVector = { 4 };
			vector<vector<int>> expectedAllFollowStack = vector<vector<int>>();
			expectedAllFollowStack.push_back(tempVector);

			parser.setParentVector(expectedParentVector);
			parser.setCurrentFollowVector(testFollowVector);
			parser.setAllFollowStack(expectedAllFollowStack);

			result = parser.handleAssignment("myvar		=	((a+b)*d) *c *((( d + e ) * f) * (g+h))");
			Assert::AreEqual(result, 0, L"incorrect", LINE_INFO());
			bool equalFollowVector = parser.getCurrentFollowVector() == expectedFollowVector;
			bool equalParentVector = parser.getParentVector() == expectedParentVector;
			bool equalAllFollowStack = parser.getAllFollowStack() == expectedAllFollowStack;
			Assert::IsTrue(equalFollowVector, L"incorrect", LINE_INFO());
			Assert::IsTrue(equalParentVector, L"incorrect", LINE_INFO());
			Assert::IsTrue(equalAllFollowStack, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestHandleRead) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			int result;

			parser.setStatementNumber(7);
			vector<int> testFollowVector = { 5, 6 };
			vector<int> expectedFollowVector = vector<int>(testFollowVector);
			expectedFollowVector.push_back(7);
			vector<int> expectedParentVector = { 1, 2 };

			vector<int> tempVector = { 4 };
			vector<vector<int>> expectedAllFollowStack = vector<vector<int>>();
			expectedAllFollowStack.push_back(tempVector);
			
			parser.setParentVector(expectedParentVector);
			parser.setCurrentFollowVector(testFollowVector);
			parser.setAllFollowStack(expectedAllFollowStack);

			result = parser.handleAssignment("  read   something  ;");
			Assert::AreEqual(result, 0, L"incorrect", LINE_INFO());
			bool equalFollowVector = parser.getCurrentFollowVector() == expectedFollowVector;
			bool equalParentVector = parser.getParentVector() == expectedParentVector;
			bool equalAllFollowStack = parser.getAllFollowStack() == expectedAllFollowStack;
			Assert::IsTrue(equalFollowVector, L"incorrect", LINE_INFO());
			Assert::IsTrue(equalParentVector, L"incorrect", LINE_INFO());
			Assert::IsTrue(equalAllFollowStack, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestHandlePrint) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			int result;

			parser.setStatementNumber(7);
			vector<int> testFollowVector = { 5, 6 };
			vector<int> expectedFollowVector = vector<int>(testFollowVector);
			expectedFollowVector.push_back(7);
			vector<int> expectedParentVector = { 1, 2 };

			vector<int> tempVector = { 4 };
			vector<vector<int>> expectedAllFollowStack = vector<vector<int>>();
			expectedAllFollowStack.push_back(tempVector);

			parser.setParentVector(expectedParentVector);
			parser.setCurrentFollowVector(testFollowVector);
			parser.setAllFollowStack(expectedAllFollowStack);

			result = parser.handleAssignment("	print   otherstuff  ;");
			Assert::AreEqual(result, 0, L"incorrect", LINE_INFO());
			bool equalFollowVector = parser.getCurrentFollowVector() == expectedFollowVector;
			bool equalParentVector = parser.getParentVector() == expectedParentVector;
			bool equalAllFollowStack = parser.getAllFollowStack() == expectedAllFollowStack;
			Assert::IsTrue(equalFollowVector, L"incorrect", LINE_INFO());
			Assert::IsTrue(equalParentVector, L"incorrect", LINE_INFO());
			Assert::IsTrue(equalAllFollowStack, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestHandleWhile) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			int result;
			
			vector<int> testFollowVector = { 6, 7 };
			vector<int> expectedFollowVector = vector<int>();

			vector<vector<int>> testAllFollowStack = vector<vector<int>>();
			vector<int> tempVector = { 2 };
			testAllFollowStack.push_back(tempVector);
			vector<vector<int>> expectedAllFollowStack = vector<vector<int>>(testAllFollowStack);
			tempVector = { 6, 7, 8 };
			expectedAllFollowStack.push_back(tempVector);

			vector<int> testParentVector = { 1 };
			vector<int> expectedParentVector = vector<int>(testParentVector);
			expectedParentVector.push_back(8);

			parser.setStatementNumber(8);
			parser.setCurrentFollowVector(testFollowVector);
			parser.setParentVector(testParentVector);
			parser.setAllFollowStack(testAllFollowStack);

			parser.setStatementNumber(8);
			parser.setCurrentFollowVector(testFollowVector);
			parser.setParentVector(testParentVector);
			parser.setAllFollowStack(testAllFollowStack);

			result = parser.handleWhile("while   (( a >= b ) || ((b == c ) && (d != e ) ) ){");
			//check that success is returned, parent vector is updated
			//follow vectors are updated
			Assert::AreEqual(result, 0, L"incorrect", LINE_INFO());
			bool equalFollowVector = parser.getCurrentFollowVector() == expectedFollowVector;
			bool equalParentVector = parser.getParentVector() == expectedParentVector;
			bool equalAllFollowStack = parser.getAllFollowStack() == expectedAllFollowStack;
			Assert::IsTrue(equalFollowVector, L"incorrect", LINE_INFO());
			Assert::IsTrue(equalParentVector, L"incorrect", LINE_INFO());
			Assert::IsTrue(equalAllFollowStack, L"incorrect", LINE_INFO());
			}

		TEST_METHOD(TestHandleIf) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			int result;
			//setup
			vector<int> testFollowVector = { 6, 7 };
			vector<int> expectedFollowVector = vector<int>();
			expectedFollowVector.clear();

			vector<vector<int>> testAllFollowStack = vector<vector<int>>();
			vector<int> tempVector = { 2 };
			testAllFollowStack.push_back(tempVector);
			vector<vector<int>> expectedAllFollowStack = vector<vector<int>>(testAllFollowStack);
			tempVector.clear();
			tempVector = { 6, 7, 8 };
			expectedAllFollowStack.push_back(tempVector);

			vector<int> testParentVector = { 1 };
			vector<int> expectedParentVector = vector<int>(testParentVector);
			expectedParentVector.push_back(8);

			parser.setStatementNumber(8);
			parser.setCurrentFollowVector(testFollowVector);
			parser.setParentVector(testParentVector);
			parser.setAllFollowStack(testAllFollowStack);

			result = parser.handleIf("if (((a<b)||(b<c)) && (!(d==e))) then  {");
			//check that success is returned, parent vector is updated
			//follow vectors are updated
			Assert::AreEqual(result, 0, L"incorrect", LINE_INFO());
			bool equalFollowVector = parser.getCurrentFollowVector() == expectedFollowVector;
			bool equalParentVector = parser.getParentVector() == expectedParentVector;
			bool equalAllFollowStack = parser.getAllFollowStack() == expectedAllFollowStack;
			Assert::IsTrue(equalFollowVector, L"incorrect", LINE_INFO());
			Assert::IsTrue(equalParentVector, L"incorrect", LINE_INFO());
			Assert::IsTrue(equalAllFollowStack, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestHandleCalls) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			int result;

			parser.setStatementNumber(7);
			vector<int> testFollowVector = { 5, 6 };
			vector<int> expectedFollowVector = vector<int>(testFollowVector);
			expectedFollowVector.push_back(7);
			vector<int> expectedParentVector = { 1, 2 };

			vector<int> tempVector = { 4 };
			vector<vector<int>> expectedAllFollowStack = vector<vector<int>>();
			expectedAllFollowStack.push_back(tempVector);

			unordered_map<int, string> expectedProcCalledByTable;
			expectedProcCalledByTable.insert({ 7, "proc2" });

			parser.setParentVector(expectedParentVector);
			parser.setCurrentFollowVector(testFollowVector);
			parser.setAllFollowStack(expectedAllFollowStack);

			result = parser.handleCall("	call proc2  ;");
			Assert::AreEqual(result, 0, L"incorrect", LINE_INFO());
			bool equalFollowVector = parser.getCurrentFollowVector() == expectedFollowVector;
			bool equalParentVector = parser.getParentVector() == expectedParentVector;
			bool equalAllFollowStack = parser.getAllFollowStack() == expectedAllFollowStack;
			bool equalProcCalledByTable = parser.getProcCalledByTable() == expectedProcCalledByTable;
			Assert::IsTrue(equalFollowVector, L"incorrect", LINE_INFO());
			Assert::IsTrue(equalParentVector, L"incorrect", LINE_INFO());
			Assert::IsTrue(equalAllFollowStack, L"incorrect", LINE_INFO());
			Assert::IsTrue(equalProcCalledByTable, L"incorrect", LINE_INFO());

		}
	};
}