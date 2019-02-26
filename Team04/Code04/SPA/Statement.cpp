#include <iostream>

#include "Statement.h"

Statement::Statement(const std::string& value) :VALUE(value) {

}

Statement::Statement(const StatementType& type, const std::string& value)
	: type(type), VALUE(value) {
}
