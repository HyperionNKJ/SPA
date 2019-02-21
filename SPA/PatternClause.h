#pragma once

#include <string>
#include "ParameterType.h"

class PatternClause {
public:
	PatternClause();

	std::string synonym;

	ParameterType paramOneType;
	ParameterType paramTwoType;

	std::string paramOneValue;
	std::string paramTwoValue;
};
