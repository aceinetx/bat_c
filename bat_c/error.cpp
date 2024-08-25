#include <iostream>
#include <string>
#include "token.h"

using std::cout;

void throw_error(std::string description, Token& faulty_token) {
	cout << "\x1b[38;5;1mCompile error at line \x1b[1m" << faulty_token.line << "\x1b[0m:\n";
	cout << description << "\n";
	throw std::exception("Compile error");
}