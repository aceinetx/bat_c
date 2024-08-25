#include "token.h"
#include "token_chars.h"
#include "configuration.h"
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <iostream>

template<typename T>
bool contains(T *arr, T val) {
	bool result = false;
	
	int i = 0;
	while (true) {
		T _val = arr[i];
		if (!_val) break;

		if (val == _val) {
			result = true;
		}

		i++;
	}

	return result;
};

void append_current_identifier(tokenarray_t& token_arr, std::string& current_identifier) {

	if (current_identifier == "def") {
		token_arr.push_back(Token(TokenType::DEFINE));
	}
	else if (current_identifier == "return") {
		token_arr.push_back(Token(TokenType::RETURN));
	}
	else if (current_identifier == "port") {
		token_arr.push_back(Token(TokenType::PORT));
	}
	else if (current_identifier == "increment") {
		token_arr.push_back(Token(TokenType::INCREMENT));
	}
	else if (current_identifier == "decrement") {
		token_arr.push_back(Token(TokenType::DECREMENT));
	}
	else if (current_identifier == "if") {
		token_arr.push_back(Token(TokenType::IF));
	}
	else if (current_identifier == "endif") {
		token_arr.push_back(Token(TokenType::ENDIF));
	}
	else if (current_identifier == "cmpexpr") {
		token_arr.push_back(Token(TokenType::COMPARE_EXPR));
	}
	else if (current_identifier == "ref") {
		token_arr.push_back(Token(TokenType::REF));
	}
	else {
		size_t len = current_identifier.length();
		char* c_ident = new char[len+1];
		memset(c_ident, 0, len+1);
		memcpy(c_ident, current_identifier.c_str(), len);
		token_arr.push_back(Token(TokenType::IDENTIFIER, c_ident));
	}

	current_identifier.clear();
}

tokenarray_t Tokenize(std::string code) {
	tokenarray_t result;

	std::string current_identifier = "";
	register_t current_number = 0;

	char prev_contains_id = 0;

	int line=1;
	bool in_string = false;

	for (int i = 0; i < code.size(); i++) {
		char c = code.at(i);
		char contains_id = 0;
		if (contains<char>((char*)IDENTIFIER_CHARS, c)) contains_id = 1;
		if (contains<char>((char*)NUMBER_CHARS, c)) contains_id = 2;
		if (contains<char>((char*)SBRACKET_CHARS, c)) contains_id = 3;
		if (contains<char>((char*)PAREN_CHARS, c)) contains_id = 4;

		if (c == '!' && code.at(i + 1) == '=') {
			result.push_back(Token(TokenType::NOTEQUALTO));
			i++;
			continue;
		}
		else if (c == '=' && code.at(i + 1) == '=') {
			result.push_back(Token(TokenType::EQUALTO));
			i++;
			continue;
		}
		else if (c == '<' && code.at(i + 1) == '=') {
			result.push_back(Token(TokenType::LESSEQUAL));
			i++;
			continue;
		}
		else if (c == '>' && code.at(i + 1) == '=') {
			result.push_back(Token(TokenType::GREATEREQUAL));
			i++;
			continue;
		}
		else if (c == '>') result.push_back(Token(TokenType::GREATER));
		else if (c == '<') result.push_back(Token(TokenType::LESS));

		if (c == '\n') {
			contains_id = -1;
			line++;
		}
		else if (c == '=') result.push_back(Token(TokenType::EQUAL));
		else if (c == '+') result.push_back(Token(TokenType::PLUS));
		else if (c == '-') result.push_back(Token(TokenType::MINUS));
		else if (c == '*') result.push_back(Token(TokenType::MUL));
		else if (c == '/') result.push_back(Token(TokenType::DIV));


		switch (contains_id) {
		case 1:
			current_identifier.push_back(c);
			break;
		case 2:
			current_number *= 10;
			current_number += c - 48;
			break;
		case 3:
			if (!current_identifier.empty()) {
				append_current_identifier(result, current_identifier);
			}

			if (c == '{') result.push_back(Token(TokenType::LSBRACKET));
			else if (c == '}') result.push_back(Token(TokenType::RSBRACKET));

			break;

		case 4:
			if (!current_identifier.empty()) {
				append_current_identifier(result, current_identifier);
			}

			if (c == '(') result.push_back(Token(TokenType::LPAREN));
			else if (c == ')') result.push_back(Token(TokenType::RPAREN));

			
			break;
		case 0:
			bool should_end = false;
			if (c == ' ') should_end = true;
			if (c == ';') should_end = true;

			if (should_end && !current_identifier.empty() && prev_contains_id == 1) {
				append_current_identifier(result, current_identifier);
			}
			else if (should_end && prev_contains_id == 2) {
				result.push_back(Token(TokenType::NUMBER, current_number));
				current_number = 0;
			}

			if (c == ';') result.push_back(Token(TokenType::ENDL));
			break;
		}
		prev_contains_id = contains_id;

		if (contains_id != -1 && !result.empty()) {
			result.back().line = line;
		}
	}
	
	return result;
}