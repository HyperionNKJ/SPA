#include <string>
#include "DesignEntityType.h"

DesignEntityType::DesignEntityType() {

}

Type DesignEntityType::getType(const std::string& designEntityTypeString) {
	if (designEntityTypeString == "stmt") {
		return Type::STATEMENT;
	}

	if (designEntityTypeString == "read") {
		return Type::READ;
	}

	if (designEntityTypeString == "print") {
		return Type::PRINT;
	}

	if (designEntityTypeString == "while") {
		return Type::WHILE;
	}

	if (designEntityTypeString == "if") {
		return Type::IF;
	}

	if (designEntityTypeString == "assign") {
		return Type::ASSIGN;
	}

	if (designEntityTypeString == "variable") {
		return Type::VARIABLE;
	}

	if (designEntityTypeString == "constant") {
		return Type::CONSTANT;
	}

	return Type::PROCEDURE;
}
