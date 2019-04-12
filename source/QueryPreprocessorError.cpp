#include "QueryPreprocessorError.h"

QueryPreprocessorError::QueryPreprocessorError(const ErrorType& errorType)
	: IS_BOOLEAN_RESULT_CL(false), ERROR_TYPE(errorType) {
}

QueryPreprocessorError::QueryPreprocessorError(const ProcessedQuery& query, const ErrorType& errorType)
	: ERROR_TYPE(errorType), 
	IS_BOOLEAN_RESULT_CL(query.resultClElemList.size() == 1 && query.resultClElemList[0].isType(Type::BOOLEAN)) {
}

bool QueryPreprocessorError::isBooleanResultClause() const {
	return IS_BOOLEAN_RESULT_CL;
}

bool QueryPreprocessorError::isSemanticError() const {
	return ERROR_TYPE == ErrorType::SEMANTIC;
}

bool QueryPreprocessorError::isSyntaticError() const {
	return ERROR_TYPE == ErrorType::SEMANTIC;
}
