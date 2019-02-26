#pragma once

#include <string>

enum class StatementType {
	DECLARATION, SELECT
};

class Statement {
public:
	const std::string VALUE;
	StatementType type;

	Statement(const std::string& value);
	Statement(const StatementType& type, const std::string& value);

	void setStatementType(StatementType& statementType) {
		type = statementType;
	}
};