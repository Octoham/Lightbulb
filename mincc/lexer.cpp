#include "lexer.h"

std::vector<Token> Lexer::Lex()
{
	std::vector<Token> tokenArray = std::vector<Token>();

	bool lexing = true;
	while (lexing)
	{
		printf("At index %d\n", position);
		printf("At line %d\n", line);
		tokenArray.insert(tokenArray.end(), GetNextToken());
		//printf("Got token of type \"%s\"\n", TokenTypeToString(tokenarray.tokens[tokenarray.count].type));
		//printf("Contains \"%s\"\n", tokenarray.tokens[tokenarray.count].contents);

		if (tokenArray[tokenArray.size() - 1].type == TOKEN_EOF)
		{
			lexing = false;
		}
	}

	position = 0; // reset position after parsing
	return tokenArray;
}

Token Lexer::GetNextToken()
{
	// TODO implement
	return Token();
}
