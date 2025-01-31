#pragma once

#include <vector>
#include <iostream>

#include "token.h"

class Lexer
{
	int position;
	int line;
	std::string sourceCode;
	std::string path;
	std::vector<Token> tokens;
	Token GetNextToken();
	bool CharInStringVector(char chr, std::vector<std::string> arr);
	bool StringInStringVector(std::string str, std::vector<std::string> arr);
	bool CharInCharVector(char chr, std::vector<char> arr);
	void PrintError(std::string err);
public:
	Lexer(std::string src, std::string path);
	std::vector<Token> Lex();
	void FixLineEnd();
	void Preprocess();
	std::string GetSource();
	void PrintTokens(std::vector<Token>& tokens);
	void PrintTokens();
};