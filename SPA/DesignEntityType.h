#pragma once

enum class Type {
	STATEMENT, READ, PRINT, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE
};

class DesignEntityType {
public:
	DesignEntityType();

	static Type getType(const std::string& designEntityTypeString);
};
