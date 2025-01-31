#pragma once

#include <string>
#include <vector>

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

namespace token
{
    static std::string string_seperator = "\"";
    static std::string char_seperator = "\'";
    static std::vector<std::string> seperators = { "(", ")", "[", "]", "{", "}", ",", ".", ":", ";" };
    static std::vector<char> seperatorChars;
    static std::vector<std::string> operators = { "=", "+", "-", "*", "/", "%", "++", "--", "<", ">", "<=", ">=", "==", "!=" };
    static std::vector<char> operatorChars;
    static std::vector<std::string> keywords = { "int", "void", "return" };
}