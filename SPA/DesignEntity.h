#pragma once
#include <string>

using namespace std;

enum Type {
	ALL, STATEMENT, READ, PRINT, CALL, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE, CODE
};

class DesignEntity {
private:
	string value;
	Type type;

public:
	DesignEntity();
	~DesignEntity();
	DesignEntity(string, Type);

	void setValue(string);
	void setType(Type);
	string getValue();
	Type getType();
};