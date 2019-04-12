#pragma once
#include <exception>
#include "ProcessedQuery.h"

enum ErrorType {
	SEMANTIC, SYNTACTIC
};

struct QueryPreprocessorError : public std::exception {
	const bool IS_BOOLEAN_RESULT_CL;
	const ErrorType& ERROR_TYPE;

	QueryPreprocessorError(const ErrorType& ERROR_TYPE);
	QueryPreprocessorError(const ProcessedQuery& QUERY, const ErrorType& ERROR_TYPE);

	bool isBooleanResultClause() const;
	bool isSemanticError() const;
	bool isSyntaticError() const;
};
