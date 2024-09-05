#include "lexer.h"

std::vector<Token> Lexer::Lex()
{
	std::vector<Token> tokenArray = std::vector<Token>();

	bool lexing = true;
	while (lexing)
	{
		//printf("At index %d\n", position);
		//printf("At line %d\n", line);
		tokenArray.push_back(GetNextToken());
		//printf("Got token of type \"%s\"\n", TokenTypeToString(tokenarray.tokens[tokenarray.count].type));
		//printf("Contains \"%s\"\n", tokenarray.tokens[tokenarray.count].contents);

		if (tokenArray[tokenArray.size() - 1].type == TOKEN_EOF)
		{
			lexing = false;
		}
	}

	position = 0; // reset position after parsing
    tokens = tokenArray;
	return tokenArray;
}

void Lexer::FixLineEnd()
{
	int pos = 0;
	while ((pos = sourceCode.find("\r\n", pos)) != std::string::npos) {
		sourceCode.replace(pos, 2, "\n");
		pos += 1; // Move to the next character after the replacement
	}
}

std::string Lexer::GetSource()
{
	return sourceCode;
}

Token Lexer::GetNextToken() // TODO make more flexible esp with operators and make it safer lmao
{
    // Skip whitespace
    while (position < sourceCode.length() && isspace(sourceCode[position]))
    {
        if (sourceCode[position] == '\n')
        {
            line++;
        }
        position++;
    }

    // Check for end of file
    if (position >= sourceCode.length())
    {
        return Token{ TOKEN_EOF, "" };
    }

    // Check for keywords
    if (std::isalpha(sourceCode[position]) || sourceCode[position] == '_')
    {
        std::string keyword;
        while (position < sourceCode.length() && (std::isalnum(sourceCode[position]) || sourceCode[position] == '_'))
        {
            keyword += sourceCode[position];
            position++;
        }

        // Check if the keyword is a reserved word
        if (StringInStringArray(keyword, keywords))
        {
            return Token{ TOKEN_KEYWORD, keyword };
        }
        else
        {
            return Token{ TOKEN_IDENTIFIER, keyword };
        }
    }

    // Check for operators
    if (StringInStringArray(sourceCode[position], operators))
    {
        std::string operatorStr;
        operatorStr += sourceCode[position];
        position++;

        // Check for multi-character operators
        if (sourceCode[position] == '=')
        {
            operatorStr += sourceCode[position];
            position++;
        }
        else if (sourceCode[position] == '|')
        {
            if (sourceCode[position + 1] == '|')
            {
                operatorStr += sourceCode[position];
                position += 2;
            }
        }
        else if (sourceCode[position] == '&')
        {
            if (sourceCode[position + 1] == '&')
            {
                operatorStr += sourceCode[position];
                position += 2;
            }
        }

        return Token{ TOKEN_OPERATOR, operatorStr };
    }

    // Check for separators
    if (StringInStringArray(sourceCode[position], seperators))
    {
        std::string separatorStr;
        separatorStr += sourceCode[position];
        position++;

        return Token{ TOKEN_SEPERATOR, separatorStr };
    }

    // Check for literals
    if (std::isdigit(sourceCode[position]))
    {
        std::string literalStr;
        while (position < sourceCode.length() && std::isdigit(sourceCode[position]))
        {
            literalStr += sourceCode[position];
            position++;
        }

        return Token{ TOKEN_LITERAL, literalStr };
    }

    // Check for character literals
    if (sourceCode[position] == char_seperator[0])
    {
        position++;
        std::string charLiteralStr;
        charLiteralStr += sourceCode[position];
        position++;
        position++; // Skip the closing quote

        return Token{ TOKEN_CHAR_LITERAL, charLiteralStr };
    }

    // Check for string literals
    if (sourceCode[position] == string_seperator[0])
    {
        position++;
        std::string stringLiteralStr;
        while (position < sourceCode.length() && sourceCode[position] != string_seperator[0])
        {
            stringLiteralStr += sourceCode[position];
            position++;
        }
        position++; // Skip the closing quote

        return Token{ TOKEN_STRING_LITERAL, stringLiteralStr };
    }

    // If none of the above, return an empty token and augment position
    std::string unknownTokenString;
    unknownTokenString += sourceCode[position];
    position++;
    return Token{ TOKEN_UNKNOWN, unknownTokenString};
}

void Lexer::PrintTokens(std::vector<Token>& tokens) {
    for (Token& token : tokens) {
        std::cout << "Token Type: ";
        switch (token.type) {
        case TOKEN_EOF:
            std::cout << "End of File";
            break;
        case TOKEN_IDENTIFIER:
            std::cout << "Identifier";
            break;
        case TOKEN_KEYWORD:
            std::cout << "Keyword";
            break;
        case TOKEN_OPERATOR:
            std::cout << "Operator";
            break;
        case TOKEN_SEPERATOR:
            std::cout << "Separator";
            break;
        case TOKEN_LITERAL:
            std::cout << "Literal";
            break;
        case TOKEN_CHAR_LITERAL:
            std::cout << "Character Literal";
            break;
        case TOKEN_STRING_LITERAL:
            std::cout << "String Literal";
            break;
        default:
            std::cout << "Unknown";
            break;
        }
        std::cout << ", Value: " << token.value << std::endl;
    }
}

void Lexer::PrintTokens() {
    for (Token& token : tokens) {
        std::cout << "Token Type: ";
        switch (token.type) {
        case TOKEN_EOF:
            std::cout << "End of File";
            break;
        case TOKEN_IDENTIFIER:
            std::cout << "Identifier";
            break;
        case TOKEN_KEYWORD:
            std::cout << "Keyword";
            break;
        case TOKEN_OPERATOR:
            std::cout << "Operator";
            break;
        case TOKEN_SEPERATOR:
            std::cout << "Separator";
            break;
        case TOKEN_LITERAL:
            std::cout << "Literal";
            break;
        case TOKEN_CHAR_LITERAL:
            std::cout << "Character Literal";
            break;
        case TOKEN_STRING_LITERAL:
            std::cout << "String Literal";
            break;
        default:
            std::cout << "Unknown";
            break;
        }
        std::cout << ", Value: " << token.value << std::endl;
    }
}

bool Lexer::StringInStringArray(char chr, std::string arr[])
{
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        if (chr == arr[i][0])
        {
            return true;
        }
    }
    return false;
}

bool Lexer::StringInStringArray(std::string str, std::string arr[])
{
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        if (str == arr[i])
        {
            return true;
        }
    }
    return false;
}
