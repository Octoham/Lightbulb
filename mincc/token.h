#pragma once

#include <string>

// Token types
enum TokenType
{
    TOKEN_UNKNOWN,
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_SEPERATOR,
    TOKEN_LITERAL,
    TOKEN_CHAR_LITERAL,
    TOKEN_STRING_LITERAL,
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
};

static std::string string_seperator = "\"";
static std::string char_seperator = "\'";
static std::string seperators[] = { "(", ")", "[", "]", "{", "}", ",", "", ":", "; "};
static std::string operators[] = { "", "" };
static std::string keywords[] = { "int", "void", "return" };