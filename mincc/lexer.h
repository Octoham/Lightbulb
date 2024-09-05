#pragma once

#include <vector>
#include <iostream>

#include "token.h"

class Lexer
{
	int position;
	int line;
	std::string sourceCode;
	std::vector<Token> tokens;
	Token GetNextToken();
	bool StringInStringArray(std::string str, std::string* arr);
	bool StringInStringArray(char chr, std::string* arr);
public:
	Lexer(std::string src) : sourceCode(src), position(0), line(1) {}
	std::vector<Token> Lex();
	void FixLineEnd();
	std::string GetSource();
	void PrintTokens(std::vector<Token>& tokens);
	void PrintTokens();
};