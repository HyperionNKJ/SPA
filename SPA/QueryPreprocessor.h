#pragma once

class QueryPreprocessor {
public:
	QueryPreprocessor(const std::string& query);
	bool parse();
private:
	const std::string& QUERY;

	bool tokenize();
	bool validateSyntax();
};
