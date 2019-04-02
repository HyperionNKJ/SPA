#include <iostream>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryPreprocessorFormatter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQueryPreprocessorFormatter) {
	public:
		TEST_METHOD(leadingSpaceRemoved) {
			std::string original = "	\n Select a;";
			std::string expected = "Select a;";

			QueryPreprocessorFormatter formatter(original);

			std::string result = formatter.getFormattedQuery();

			Assert::IsTrue(result == expected);
		}
		TEST_METHOD(trailingSpaceRemoved) {
			std::string original = "Select a;	\n ";
			std::string expected = "Select a;";

			QueryPreprocessorFormatter formatter(original);

			std::string result = formatter.getFormattedQuery();

			Assert::IsTrue(result == expected);
		}
	};
}