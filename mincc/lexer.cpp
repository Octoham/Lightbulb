#include "lexer.h"

using namespace token;

Lexer::Lexer(std::string src, std::string path) : sourceCode(src), path(path), position(0), line(1)
{
    // populate chars
    for (int i = 0; i < std::size(operators); i++)
    {
        for (int j = 0; j < std::size(operators[i]); j++)
        {
            if (std::find(operatorChars.begin(), operatorChars.end(), operators[i][j]) == operatorChars.end()) // if char not in vector
            {
                operatorChars.push_back(operators[i][j]);
            }
        }
    }
    for (int i = 0; i < std::size(seperators); i++)
    {
        for (int j = 0; j < std::size(seperators[i]); j++)
        {
            if (std::find(seperatorChars.begin(), seperatorChars.end(), seperators[i][j]) == seperatorChars.end()) // if char not in vector
            {
                seperatorChars.push_back(seperators[i][j]);
            }
        }
    }
    std::string ocStr(operatorChars.begin(), operatorChars.end());
    //std::cout << ocStr << std::endl;
    std::string scStr(seperatorChars.begin(), seperatorChars.end());
    //std::cout << scStr << std::endl;
}

std::vector<Token> Lexer::Lex()
{
	std::vector<Token> tokenArray = std::vector<Token>();

    //std::cout << "Source Code btw: \n" << sourceCode << std::endl;
    /*
    {
        std::cout << "Running tests!" << std::endl;
        bool works = true;
        if (!StringInStringArray("return", keywords)) works = false;
        if (!StringInStringArray("+", operators)) works = false;
        if (!StringInStringArray("]", seperators)) works = false;
        if (StringInStringArray("a", keywords)) works = false;
        if (StringInStringArray("a", operators)) works = false;
        if (StringInStringArray("a", seperators)) works = false;
        if (works) std::cout << "IT WORKS!!" << std::endl;
        else std::cout << "IT DOESN'T WORK!!" << std::endl;
    }
    */
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
		pos++; // Move to the next character after the replacement
	}
    //std::cout << "Fixed line endings" << std::endl;
}

void Lexer::Preprocess()
{
    int pos = 0;
    while (pos < sourceCode.size())
    {
        if (sourceCode[pos] == '#')
        {
            while (sourceCode[pos] != '\n')
            {
                sourceCode.replace(pos, 1, " ");
                pos++;
            }
        }
        pos++;
    }
}

std::string Lexer::GetSource()
{
	return sourceCode;
}

Token Lexer::GetNextToken() // TODO make it safer lmao
{
    // Skip whitespace and newline
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
        if (StringInStringVector(keyword, keywords))
        {
            return Token{ TOKEN_KEYWORD, keyword };
        }
        else
        {
            return Token{ TOKEN_IDENTIFIER, keyword };
        }
    }

    // Check for operators
    if (CharInCharVector(sourceCode[position], operatorChars))
    {
        std::string operatorStr;
        while (position < sourceCode.length() && CharInCharVector(sourceCode[position], operatorChars))
        {
            operatorStr += sourceCode[position];
            position++;
        }

        // Check if the operators is valid
        if (StringInStringVector(operatorStr, operators))
        {
            return Token{ TOKEN_OPERATOR, operatorStr };
        }
        for (int i = operatorStr.length() - 1; i > 0; i--) // check for smaller operators
        {
            operatorStr = operatorStr.substr(0, i); // progressively trim the operatorStr to try to find a match
            position--; // and setback position to not forget characters
            if (StringInStringVector(operatorStr, operators))
            {
                return Token{ TOKEN_OPERATOR, operatorStr };
            }

        }
        // TODO lexer error when unknown operator
        return Token{ TOKEN_UNKNOWN, operatorStr };
    }

    // Check for separators
    if (CharInCharVector(sourceCode[position], seperatorChars))
    {
        std::string separatorStr;
        while (position < sourceCode.length() && CharInCharVector(sourceCode[position], seperatorChars))
        {
            separatorStr += sourceCode[position];
            position++;
        }

        // Check if the seperator is valid
        if (StringInStringVector(separatorStr, seperators))
        {
            return Token{ TOKEN_SEPERATOR, separatorStr };
        }
        for (int i = separatorStr.length() - 1; i > 0; i--) // check for smaller seperators
        {
            separatorStr = separatorStr.substr(0, i); // progressively trim the seperatorStr to try to find a match
            position--; // and setback position to not forget characters
            if (StringInStringVector(separatorStr, seperators))
            {
                return Token{ TOKEN_SEPERATOR, separatorStr };
            }
            
        }
        // TODO lexer error when unknown seperator
        return Token{ TOKEN_UNKNOWN, separatorStr };

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
        // TODO do more checks to ensure that there is a closing quote
        // TODO implement escape characters
        // TODO lexer error when char doesn't end
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
        // TODO implement escape characters
        // TODO lexer error when string doesn't close
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
    // TODO lexer error when unknown character
    std::string unknownTokenString;
    unknownTokenString += sourceCode[position];
    position++;
    return Token{ TOKEN_UNKNOWN, unknownTokenString };
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
    PrintTokens(tokens);
}

bool Lexer::CharInStringVector(char chr, std::vector<std::string> arr)
{
    for (int i = 0; i < std::size(arr); i++)
    {
        if (chr == arr[i][0])
        {
            return true;
        }
    }
    return false;
}

bool Lexer::StringInStringVector(std::string str, std::vector<std::string> arr)
{   
    for (int i = 0; i < std::size(arr); i++)
    {
        if (str == arr[i])
        {
            return true;
        }
    }
    return false;
}

bool Lexer::CharInCharVector(char chr, std::vector<char> arr)
{
    for (int i = 0; i < std::size(arr); i++)
    {
        if (chr == arr[i])
        {
            return true;
        }
    }
    return false;
}

void Lexer::PrintError(std::string err)
{
    std::cout << path << "(" << line << ") Lexer error: " << err << std::endl;
}
