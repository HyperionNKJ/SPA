#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
	if (wrapper == 0) wrapper = new TestWrapper;
	return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
	// create any objects here as instance variables of this class
	// as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	int result = parser.parse(filename, pkb);
	if (result != 0) {
		cout << parser.getErrorMessage() << endl;
		exit(1);
	}
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
	results = queryProcessor.evaluate(query, pkb);

	// call your evaluator to evaluate the query here
	// ...code to evaluate query...

	// store the answers to the query in the results list (it is initially empty)
	// each result must be a string.
}
