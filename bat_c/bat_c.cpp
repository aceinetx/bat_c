#include <iostream>
#include <fstream>
#include "token.h"
#include "tokenizer.h"
#include "compiler.h"
#include "configuration.h"
#include <stdexcept>
#include <locale.h>
#include <fmt/core.h>

using std::cout;

int main(int argc, char** argv)
{
	try {
		setlocale(LC_ALL, "");
		if (argc < 2) {
			throw std::exception("No filename provided");
		}

		std::string code;

		std::ifstream t(argv[1]);

		t.seekg(0, std::ios::end);
		code.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		code.assign((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());

		std::string assembly = "";

		tokenarray_t tokens = Tokenize(code);
		if (SHOW_PSEUDOCODE) {
			for (int i = 0; i < tokens.size(); i++) {
				Token& token = tokens.at(i);
				std::string x;
				if (token.type == IDENTIFIER) x.append(token.value);
				if (token.type == CHAR) x = "chr";
				if (token.type == NUMBER) x.append(std::to_string(token.value_long));
				if (token.type == RETURN) x = "return";
				if (token.type == LPAREN) x = "(";
				if (token.type == RPAREN) x = ")";
				if (token.type == LSBRACKET) x = "{";
				if (token.type == RSBRACKET) x = "}";
				if (token.type == ENDL) x = ";";
				if (token.type == DEFINE) x = "def";
				if (token.type == INCREMENT) x = "inc";
				if (token.type == DECREMENT) x = "dec";
				if (token.type == PLUS) x = "+";
				if (token.type == MINUS) x = "-";
				if (token.type == MUL) x = "*";
				if (token.type == DIV) x = "/";
				if (token.type == EQUAL) x = "=";
				if (token.type == EQUALTO) x = "==";
				if (token.type == GREATER) x = ">";
				if (token.type == LESS) x = "<";
				if (token.type == GREATEREQUAL) x = ">=";
				if (token.type == LESSEQUAL) x = "<=";
				if (token.type == COMPARE_EXPR) x = "cmpexpr";
				if (token.type == IF) x = "if";
				if (token.type == ENDIF) x = "endif";
				if (token.type == NOTEQUALTO) x = "!=";
				if (token.type == REF) x = "ref";
				if (token.type == PORT) x = "port";
				cout << x << " ";
			}
			cout << "\n";
			cout << "===============\nFinal assembly:\n";
		}

		assembly += "jmp .batc_execute_and_halt\n";

		compile(tokens, assembly);

		assembly += ".batc_execute_and_halt\ncal .main\nhlt";


		// free the heap allocated values
		for (int i = 0; i < tokens.size(); i++) {
			Token& token = tokens.at(i);
			if (token.need_to_free) {
				delete token.value;
			}
		}

		cout << assembly << "\n";

	}
	catch (std::exception& e) {
		fmt::print("{} C++ Exception thrown: {}", "▪", e.what());
		exit(3);
	}
}