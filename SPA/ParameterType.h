#pragma once

enum class ParameterType {
	STATEMENT, READ, PRINT, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE,
	UNDERSCORE, SYNONYM, INVALID, FIXED_STATEMENT, FIXED_VAR, FIXED_CONSTANT,
	FIXED_VAR_NAME, FIXED_CONST_VALUE
};