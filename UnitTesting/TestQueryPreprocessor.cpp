#include <iostream>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryPreprocessorFormatter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQueryPreprocessor) {
	public:
		TEST_METHOD(leadingWhitespaceRemoved) {
			std::string original = "\t\n\v\f\r          Select a;";
			std::string expected = "Select a;";

			QueryPreprocessorFormatter formatter(original);
			std::string result = formatter.getFormattedQuery();

			Assert::IsTrue(result == expected);
		}
		TEST_METHOD(trailingWhitespaceRemoved) {
			std::string original = "Select a;          \t\n\v\f\r";
			std::string expected = "Select a;";

			QueryPreprocessorFormatter formatter(original);
			std::string result = formatter.getFormattedQuery();

			Assert::IsTrue(result == expected);
		}
		TEST_METHOD(extraWhitespaceRemoved) {
			std::string original = "Select\t\n\v\f\r          a;";
			std::string expected = "Select a;";

			QueryPreprocessorFormatter formatter(original);
			std::string result = formatter.getFormattedQuery();

			Assert::IsTrue(result == expected);
		}
		TEST_METHOD(allExtraWhitespaceRemoved) {
			std::string original = "\t\n\v\f\r          Select\t\n\v\f\r          a;\t\n\v\f\r          ";
			std::string expected = "Select a;";

			QueryPreprocessorFormatter formatter(original);
			std::string result = formatter.getFormattedQuery();

			Assert::IsTrue(result == expected);
		}
		TEST_METHOD(spaceAfterConstantRHSWithClause) {
			std::string original = "stmt s1;\nassign a2;\nvariable v;\nSelect s1 such that Follows*(a2, s1) and Uses(a2, \"v\") with v = \"v\" pattern a2( _ , _ );";
			std::string expected = "stmt s1;assign a2;variable v;Select s1 such that Follows*(a2,s1) and Uses(a2,\"v\") with v=\"v\" pattern a2(_,_);";

			QueryPreprocessorFormatter formatter(original);
			std::string result = formatter.getFormattedQuery();

			Assert::IsTrue(result == expected);
		}
	};
}
