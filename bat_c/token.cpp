#include "token.h"
#include "configuration.h"

Token::Token(TokenType type, char* value) {
	this->type = type;
	this->value = value;
	this->value_long = (register_t)value;
	this->need_to_free = false;
}

Token::Token(TokenType type, register_t value) {
	this->type = type;
	this->value = 0;
	this->value_long = value;
	this->need_to_free = false;
}

Token::~Token() = default;