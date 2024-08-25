#pragma once
#include <vector>
#include "configuration.h"
//#define TOKEN_ARR std::vector<Token>


enum TokenType {
	IDENTIFIER,
	CHAR,
	NUMBER,
	RETURN,
	LPAREN,
	RPAREN,
	LSBRACKET,
	RSBRACKET,
	ENDL,
	DEFINE,
	INTERRUPT,
	INCREMENT,
	DECREMENT,
	IF,
	ENDIF,
	EQUAL,
	PLUS,
	MINUS,
	MUL,
	DIV,
	COMPARE_EXPR,
	LESS,
	GREATER,
	EQUALTO,
	NOTEQUALTO,
	LESSEQUAL,
	GREATEREQUAL
};

class Token {

public:
	TokenType type;
	char* value;
	register_t value_long;
	bool need_to_free;
	int line=0;

public:
	Token(TokenType type, char* value=0);
	Token(TokenType type, register_t value);

	~Token();
};

typedef std::vector<Token> tokenarray_t;