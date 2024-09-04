#pragma once

#include <memory>
#include <string>

enum TokenType
{
	TokenType_IDENTIFIER,

	//Literals
	TokenType_DIGIT,
	TokenType_FLOAT,
	TokenType_STRING,
	TokenType_BOOL,

	//Operators
	TokenType_PLUS, // +
	TokenType_MINUS, // -
	TokenType_TIMES, // *
	TokenType_SLASH, // /
	TokenType_VBAR, // |
	TokenType_AMP, // &
	TokenType_PERIOD, // .
	TokenType_EQL, // =
	TokenType_ISEQL, // ==
	TokenType_NOT, // !
	TokenType_LSS, // <
	TokenType_GTR, // >
	TokenType_LEQ,
	TokenType_GEQ,

	//Seperators
	TokenType_LPAREN, // (
	TokenType_RPAREN, // )
	TokenType_LBRACK, // [
	TokenType_RBRACK, // ]
	TokenType_LBRACE, // {
	TokenType_RBRACE, // }
	TokenType_SEMICOLON, // ;
	TokenType_COLON, // :
	TokenType_COMMA, // ,
	TokenType_BECOMES,


	//Keywords
	TokenType_IF, // if
	TokenType_WHILE, // while
	TokenType_FOR, // for
	TokenType_RETURN, // return
	TokenType_DO, // do
	TokenType_NEW, // new
	TokenType_DELETE, // delete
	TokenType_NULL, // null


	//Unique
	TokenType_UNKNOWN,
	TokenType_EOF
};

struct Token
{
	TokenType type;
	int length;
	char* contents;
};

struct TokenArray
{
	Token* tokens;
	int count;
	int capacity;
};

struct Tokenizer
{
	char* string;
	int position;
	int line;
};

TokenArray LexInput(char* input);

Token GetNextToken(Tokenizer& tokenizer);

char* Preprocess(char* input);

char* CleanInput(char* input);

char* CompactInput(char* input);

int EnsureAscii(char* input, int size);

void DeleteTokenContents(TokenArray& token_array);

void DeleteTokens(TokenArray& token_array);

void InitializeTokenArray(TokenArray& token_array, unsigned int size);

void ResizeTokenArray(TokenArray& token_array, unsigned int size);

bool IsWhiteSpace(char c);

bool IsCRLF(Tokenizer& tokenizer);

bool IsEndOfLine(char c);

bool IsLetter(char c);

bool IsNumeric(char c);

char* TokenTypeToString(TokenType type);

//char* TokenToString(Token token);