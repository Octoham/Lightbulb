#pragma once

#include <string>

// Token types
enum TokenType
{
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