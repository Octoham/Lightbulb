#pragma once

#include <vector>

#include "token.h"

class Lexer
{
	int position;
	int line;
	std::string sourceCode;
	Token GetNextToken();
public:
	Lexer(std::string src) : sourceCode(src), position(0) {}
	std::vector<Token> Lex();
};