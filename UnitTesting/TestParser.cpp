#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

//statics from parser. Patchwork solution for now
static int KEY_PROCEDURE = 1;
static int KEY_ASSIGN = 2;
static int KEY_IF = 3;
static int KEY_ELSE = 4;
static int KEY_WHILE = 5;
static int KEY_READ = 6;
static int KEY_PRINT = 7;
static int KEY_CLOSE_BRACKET = 8;
static int KEY_CALL = 9;
static int WHILECONTAINER = 100;
static int IFCONTAINER = 101;
static int ELSECONTAINER = 102;

namespace UnitTesting {
	TEST_CLASS(TestParser) {
	public:
		TEST_METHOD(TestCheckProcedure) {
			Parser parser;
			bool result;

			result = parser.checkProcedure("procedure abcd {");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkProcedure("procedure abcd{");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkProcedure("procedure a {");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			//test spaces
			result = parser.checkProcedure("	procedure	abcd		 {");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			//unaccepted proc name
			result = parser.checkProcedure("procedure 123 {");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			//no proc name
			result = parser.checkProcedure("procedure {");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}
		
		TEST_METHOD(TestCheckFactor) {
			Parser parser;
			bool result;
			
			result = parser.checkFactor("Avar");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkFactor("1234");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkFactor("(a)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkFactor("aab**");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckTerm) {
			Parser parser;
			bool result;

			result = parser.checkTerm("c*d");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkTerm("c*d/e%f");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkTerm("Avar");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkTerm("a+b");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckExpr) {
			Parser parser;
			bool result;

			result = parser.checkExpr("a+b*c*(d+e)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkExpr("c*d*e");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkExpr("(a+b)*c%e*(10+15)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkExpr("8*(100+201)%5");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckAssignment) {
			Parser parser;
			bool result;

			result = parser.checkAssignment("myvar=X+Y-Z+11");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("x=x+y+z*(1+2+3+x+y+z)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("x=x*(y+1/(z+2%(a+3)))");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("123=a+b+c+d");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("=a+b+c+d");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("x=");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("x=(a+b+c+d))");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkAssignment("x=)a+b+c+d");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckRelFactor) {
			Parser parser;
			bool result;

			result = parser.checkRelFactor("somevarname");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkRelFactor("531");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkRelFactor("a+b+c*d");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkRelFactor("123notavar");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}
		TEST_METHOD(TestCheckCondExpr) {
			Parser parser;
			bool result;

			result = parser.checkCondExpr("(first>second)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("(first>=second)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("(first<=second)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("(first<second)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("(first==second)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("(first!=second)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("(!(first>second))");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("((first>second)&&(third!=fourth))");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("((first>second)||(!(third!=fourth)))");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("((a+b<=c+d)||(!(c+d!=a+b*c)))");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("(123<456)");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("((22>55)||(!(69!=134+137)))");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("((1+2==3)&&(2==5))");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("(first==second&&first>2)");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("(first==second||(first>2))");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkCondExpr("((first==second)&&first>2)");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckWhile) {
			Parser parser;
			bool result;

			result = parser.checkWhile("while ((A < B) && (C != E)) {");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkWhile("while () {");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkWhile("while (a < b) then {");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkWhile("while (a < b {");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckPrint) {
			Parser parser;
			bool result;

			result = parser.checkPrint("print myvar;");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkPrint("	print	 manyspaces;");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkPrint("print 123;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkPrint("print avar anothervar;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckRead) {
			Parser parser;
			bool result;

			result = parser.checkRead("read myvar;");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkRead("	read	 manyspaces;");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkRead("read 123;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());

			result = parser.checkRead("read avar anothervar;");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestCheckElse) {
			Parser parser;
			bool result;

			result = parser.checkElse("else    {");
			Assert::AreEqual(result, true, L"incorrect", LINE_INFO());

			result = parser.checkElse("else abcd{");
			Assert::AreEqual(result, false, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestGetStatementIntent) {
			Parser parser;
			int result;

			result = parser.getStatementIntent("procedure a{");
			Assert::AreEqual(result, KEY_PROCEDURE, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent("procedure while{");
			Assert::AreEqual(result, KEY_PROCEDURE, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent("while ((A >= B) && (b != c)) {");
			Assert::AreEqual(result, KEY_WHILE, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent("while (if <= read) {");
			Assert::AreEqual(result, KEY_WHILE, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent("if ((a == b) || (B >= C)) then {");
			Assert::AreEqual(result, KEY_IF, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent("if ((then > while) && (read+print<procedure)) {");
			Assert::AreEqual(result, KEY_IF, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent("read First;");
			Assert::AreEqual(result, KEY_READ, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent("read print;");
			Assert::AreEqual(result, KEY_READ, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent("print Second;");
			Assert::AreEqual(result, KEY_PRINT, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent("print read;");
			Assert::AreEqual(result, KEY_PRINT, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent("else		{");
			Assert::AreEqual(result, KEY_ELSE, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent(" } ");
			Assert::AreEqual(result, KEY_CLOSE_BRACKET, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent("while = if + read * print");
			Assert::AreEqual(result, KEY_ASSIGN, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent("procedure=else*(while+print)");
			Assert::AreEqual(result, KEY_ASSIGN, L"incorrect", LINE_INFO());

			result = parser.getStatementIntent("first = second*(third+fourth*(fifth-sixth)/seventh)");
			Assert::AreEqual(result, KEY_ASSIGN, L"incorrect", LINE_INFO());
		}

		TEST_METHOD(TestHandleProcedure) {
			Parser parser;
			PKB pkb;
			parser.setPKB(&pkb);
			int result;
			string expectedProc = "myProcOne";

			result = parser.handleProcedure("procedure myProcOne {");
			Assert::AreEqual(result, 0, L"incorrect", LINE_INFO());
			Assert::AreEqual(0, expectedProc.compare(parser.getCurrentProcedure()), L"incorrect", LINE_INFO());

			expectedProc = "if";
			result = parser.handleProcedure("  procedure   if{");
			Assert::AreEqual(result, 0, L"incorrect", LINE_INFO());
			Assert::AreEqual(0, expectedProc.compare(parser.getCurrentProcedure()), L"incorrect", LINE_INFO());
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
	};
}