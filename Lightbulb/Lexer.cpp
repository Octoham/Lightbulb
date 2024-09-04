#include "Lexer.h"

TokenArray LexInput(char* input)
{
	
	Tokenizer tokenizer = {};
	tokenizer.string = input;
	tokenizer.position = 0;
	tokenizer.line = 1;
	bool lexing = true;

	TokenArray tokenarray = {};
	InitializeTokenArray(tokenarray, 10);
	tokenarray.count = 0;

	while (lexing)
	{
		printf("At index %d\n", tokenizer.position);
		printf("At line %d\n", tokenizer.line);
		tokenarray.tokens[tokenarray.count] = GetNextToken(tokenizer);
		printf("Got token of type \"%s\"\n", TokenTypeToString(tokenarray.tokens[tokenarray.count].type));
		printf("Contains \"%s\"\n", tokenarray.tokens[tokenarray.count].contents);

		tokenarray.count++;

		if (tokenarray.count == tokenarray.capacity)
		{
			ResizeTokenArray(tokenarray, tokenarray.capacity * 2);
		}

		if (tokenarray.tokens[tokenarray.count - 1].type == TokenType_EOF)
		{
			lexing = false;
		}
	}

	ResizeTokenArray(tokenarray, tokenarray.count);
	return tokenarray;
}

Token GetNextToken(Tokenizer& tokenizer)
{
	loop:
	Token token = {};
	token.length = 0;

	switch (tokenizer.string[tokenizer.position])
	{
	case '\0':
	{
		token.type = TokenType_EOF;
		//char* str = new char[10];
		//strcpy(str, "EndSymbol");
		//token.contents = str;
	} break;
	case '(': token.type = TokenType_LPAREN; break;
	case ')': token.type = TokenType_RPAREN; break;
	case '{': token.type = TokenType_LBRACE; break;
	case '}': token.type = TokenType_RBRACE; break;
	case '[': token.type = TokenType_LBRACK; break;
	case ']': token.type = TokenType_RBRACK; break;

	case '+': token.type = TokenType_PLUS; break;
	case '-': token.type = TokenType_MINUS; break;
	case '*': token.type = TokenType_TIMES; break;
	case '/': token.type = TokenType_SLASH; break;
	case '|': token.type = TokenType_VBAR; break;
	case '&': token.type = TokenType_AMP; break;

	case ';': token.type = TokenType_SEMICOLON; break;
	case ':': token.type = TokenType_COLON; break;
	case ',': token.type = TokenType_COMMA; break;
	case '.': token.type = TokenType_PERIOD; break;

	case '=': token.type = TokenType_EQL; break;
	case '>': token.type = TokenType_GTR; break;
	case '<': token.type = TokenType_LSS; break;
	case '!': token.type = TokenType_NOT; break;

	case '"':
	{
		tokenizer.position++; // skip start "

		token.type = TokenType_STRING;
		int startpos = tokenizer.position;

		while (tokenizer.string[tokenizer.position] != '"')
		{
			tokenizer.position++;

			if (tokenizer.string[tokenizer.position] == '\0')
			{
				printf("Lexer Error: Unfinished string literal\nAt line %i\n", tokenizer.line);
				throw;
			}
			else if (IsEndOfLine(tokenizer.string[tokenizer.position]))
			{
				tokenizer.line++;
			}
			else if (tokenizer.string[tokenizer.position] == '\\') //skip any potential weird characters after the \ 
			{
				tokenizer.position++;
				tokenizer.position++;
				continue;
			}
		}
		token.length = tokenizer.position - startpos;
		token.contents = new char[token.length + 1];
		for (int i = 0; startpos != tokenizer.position; i++)
		{
			token.contents[i] = tokenizer.string[startpos];
			startpos++;
		}
		token.contents[token.length] = '\0';

		tokenizer.position++; // skip the end "
	} break;
	case '\'':
	{
		tokenizer.position++; // skip start '

		token.type = TokenType_STRING;
		int startpos = tokenizer.position;

		while (tokenizer.string[tokenizer.position] != '\'')
		{
			tokenizer.position++;

			if (tokenizer.string[tokenizer.position] == '\0')
			{
				printf("Lexer Error: unfinished string literal\nAt line %i\n", tokenizer.line);
				throw;
			}
			else if (IsEndOfLine(tokenizer.string[tokenizer.position]))
			{
				tokenizer.line++;
			}
		}
		token.length = tokenizer.position - startpos;
		token.contents = new char[token.length + 1];
		for (int i = 0; startpos != tokenizer.position; i++)
		{
			token.contents[i] = tokenizer.string[startpos];
			startpos++;
		}
		token.contents[token.length] = '\0';

		tokenizer.position++; // skip the end '
	} break;
	default:
	{
		if (IsLetter(tokenizer.string[tokenizer.position]) || tokenizer.string[tokenizer.position] == '_')
		{
			token.type = TokenType_IDENTIFIER;
			int startpos = tokenizer.position;

			while (IsLetter(tokenizer.string[tokenizer.position]) /* || IsNumeric(tokenizer.string[tokenizer.position]) */ || tokenizer.string[tokenizer.position] == '_')
			{
				tokenizer.position++;
				if (token.length == 255) break;
			}
			token.length = tokenizer.position - startpos;
			token.contents = new char[token.length + 1];
			for (int i = 0; startpos != tokenizer.position; i++)
			{
				token.contents[i] = tokenizer.string[startpos];
				startpos++;
			}
			token.contents[token.length] = '\0';
			tokenizer.position--; // just put the tokenizer's position at the last character of the identifier, since the end of the function pushes it forwards anyways

			// Once we have an identifier, we can check if it matches a keyword
			if (strcmp(token.contents, "for") == 0)
			{
				token.type = TokenType_FOR;
				break;
			}
			if (strcmp(token.contents, "while") == 0)
			{
				token.type = TokenType_WHILE;
				break;
			}
			if (strcmp(token.contents, "return") == 0)
			{
				token.type = TokenType_RETURN;
				break;
			}
			if (strcmp(token.contents, "do") == 0)
			{
				token.type = TokenType_DO;
				break;
			}
			if (strcmp(token.contents, "new") == 0)
			{
				token.type = TokenType_NEW;
				break;
			}
			if (strcmp(token.contents, "delete") == 0)
			{
				token.type = TokenType_DELETE;
				break;
			}
			if (strcmp(token.contents, "null") == 0)
			{
				token.type = TokenType_NULL;
				break;
			}

			// For boolean values we can change our token type
			if (strcmp(token.contents, "true") == 0 ||
				strcmp(token.contents, "false") == 0)
			{
				token.type = TokenType_BOOL;
			}
		}
		else if (IsNumeric(tokenizer.string[tokenizer.position]))
		{
			int startpos = tokenizer.position;
			token.type = TokenType_DIGIT;
			bool isFloat = false;

			while (IsNumeric(tokenizer.string[tokenizer.position]) || tokenizer.string[tokenizer.position] == '.')
			{
				tokenizer.position++;

				if (tokenizer.string[tokenizer.position] == '.')
				{
					if (isFloat)
					{
						printf("Lexer Error: Two periods in a float literal\nAt line %i\n", tokenizer.line);
						throw;
					}
					else
					{
						isFloat = true;
						token.type = TokenType_FLOAT;
						tokenizer.position++;
					}
				}
			}
			token.length = tokenizer.position - startpos;
			token.contents = new char[token.length + 1];
			for (int i = 0; startpos != tokenizer.position; i++)
			{
				token.contents[i] = tokenizer.string[startpos];
				startpos++;
			}
			token.contents[token.length] = '\0';
		}
		else if (IsWhiteSpace(tokenizer.string[tokenizer.position]))
		{
			tokenizer.position++;
			goto loop;
		}
		else if (IsEndOfLine(tokenizer.string[tokenizer.position]))
		{
			tokenizer.position++;
			tokenizer.line++;
			goto loop;
		}
		else
		{
			printf("Lexer Error: Unexpected symbol \"%c\"\nAt line %i\n", tokenizer.string[tokenizer.position], tokenizer.line);
			throw;
		}
	} break;
	}

	tokenizer.position++;
	return token;
}

char* Preprocess(char* input)
{
	if (input == NULL)
		return NULL; // Handle NULL input appropriately

	long len = strlen(input);
	len = EnsureAscii(input, len);
	char* output = (char*)malloc(sizeof(char) * len + 1);
	if (output == NULL)
		return NULL; // Memory allocation failed

	long pos = 0;
	long opos = 0;
	bool inString = false;
	bool inChar = false;

	while (input[pos] != '\0') {
		if (input[pos] == '\r' && input[pos + 1] == '\n') {
			pos++; // Skip the CR of a CRLF
		}
		else if (input[pos] == '"')
		{
			if(!inChar) inString = !inString;
		}
		else if (input[pos] == '\'')
		{
			if (!inString) inChar = !inChar;
		}
		else if (!(inString || inChar))
		{
			if (input[pos] == '/') {
				if (input[pos + 1] == '/') {
					pos++;
					pos++;
					while (!(IsEndOfLine(input[pos]) || input[pos] == '\0')) {
						pos++;
					} // Skip // comments
					continue;
				}
				else if (input[pos + 1] == '*') {
					pos++;
					pos++;
					int newlines = 0;
					while (!(input[pos - 1] == '*' && input[pos] == '/')) {
						if (input[pos] == '\n') {
							newlines++;
						}
						pos++;
					} // Skip /**/ comments while keeping track of newlines
					pos++;
					for (; newlines > 0; newlines--) {
						output[opos] = '\n';
						opos++;
					}
					continue;
				}
			}
			else if (input[pos] == '#')
			{
				pos++;
				while (!(IsEndOfLine(input[pos]) || input[pos] == '\0')) {
					pos++;
				} // Skip preprocessor lines (for now)
				continue;
			}
		}
		else if ((inString || inChar))
		{
			if (input[pos] == '\\') // skip anything that comes after \ in a string
			{
				output[opos] = input[pos];
				pos++;
				opos++;
			}
		}

		output[opos] = input[pos];
		pos++;
		opos++;
	}

	output[opos] = '\0'; // Null-terminate the output string
	return output;
}

char* CleanInput(char* input) {
	if (input == NULL)
		return NULL; // Handle NULL input appropriately

	long len = strlen(input);
	len = EnsureAscii(input, len);
	char* output = (char*)malloc(sizeof(char) * len + 1);
	if (output == NULL)
		return NULL; // Memory allocation failed

	long pos = 0;
	long opos = 0;

	while (input[pos] != '\0') {
		if (input[pos] == '\r' && input[pos + 1] == '\n') {
			pos++; // Skip the CR of a CRLF
		}
		else if (IsWhiteSpace(input[pos])) {
			bool space = true;
			bool letterLeft = false;
			if (input[pos - 1] == '\n')
			{
				space = false;
			}
			if (IsLetter(input[pos - 1]) || IsNumeric(input[pos - 1]))
			{
				letterLeft = true;
			}
			do {
				pos++;
			} while (IsWhiteSpace(input[pos])); // Skip multiple spaces
			if (!(letterLeft && (IsLetter(input[pos]) || IsNumeric(input[pos]))))
			{
				space = false;
			}
			if (space)
			{
				output[opos] = ' ';
				opos++;
			}
			continue;
		}
		else if (input[pos] == '/') {
			if (input[pos+1] == '/') {
				pos++;
				pos++;
				while (!(IsEndOfLine(input[pos]) || input[pos] == '\0')) {
					pos++;
				} // Skip // comments
				continue;
			}
			else if (input[pos+1] == '*') {
				pos++;
				pos++;
				int newlines = 0;
				while (!(input[pos - 1] == '*' && input[pos] == '/')) {
					if (input[pos] == '\n') {
						newlines++;
					}
					pos++;
				} // Skip /**/ comments while keeping track of newlines
				pos++;
				for (; newlines > 0; newlines--) {
					output[opos] = '\n';
					opos++;
				}
				continue;
			}
		}

		output[opos] = input[pos];
		pos++;
		opos++;
	}

	output[opos] = '\0'; // Null-terminate the output string
	return output;
}

char* CompactInput(char* input) {
	if (input == NULL)
		return NULL; // Handle NULL input appropriately

	long len = strlen(input);
	len = EnsureAscii(input, len);
	char* output = (char*)malloc(sizeof(char) * len + 1);
	if (output == NULL)
		return NULL; // Memory allocation failed

	long pos = 0;
	long opos = 0;

	while (input[pos] != '\0') {
		if (input[pos] == '\n' || input[pos] == '\r')
		{
			pos++;
			continue;
		}
		if (input[pos] == ' ' || input[pos] == '\t') // this is only for space removal
		{
			if ((!IsLetter(input[pos - 1]) && !IsNumeric(input[pos - 1])) || (!IsLetter(input[pos + 1]) && !IsNumeric(input[pos + 1]))) // if both are not alphanumeric
			{
				pos++;
				continue;
			}
			if (input[pos] == '\t')
			{
				input[pos] == ' ';
			}
		}

		output[opos] = input[pos];
		pos++;
		opos++;
	}

	output[opos] = '\0'; // Null-terminate the output string
	return output;
}

int EnsureAscii(char* input, int size)
{
	for (int i = 0; i < size + 1; i++)
	{
		if (input[i] < 1 || input[i] > 128)
		{
			input[i] = '\0';
			return i;
		}
	}
}

void DeleteTokenContents(TokenArray& token_array)
{
	for (int i = 0; i < token_array.count; i++)
	{
		delete[] token_array.tokens[i].contents;
	}
}

void DeleteTokens(TokenArray& token_array)
{
	DeleteTokenContents(token_array);
	delete[] token_array.tokens;
	token_array = {};
}

void InitializeTokenArray(TokenArray& token_array, unsigned int size)
{
	token_array.tokens = new Token[size];
	token_array.capacity = size;
}

void ResizeTokenArray(TokenArray& token_array, unsigned int size)
{
	if (size > token_array.capacity)
	{
		Token* tmp = (Token*)realloc(token_array.tokens, size * sizeof(Token));
		token_array.tokens = tmp;
		token_array.capacity = size;
	}
	else if (size < token_array.capacity)
	{
		TokenArray tmp;
		tmp.tokens = new Token[size];
		tmp.capacity = size;
		tmp.count = token_array.count;
		int i = 0;
		while (i < size)
		{
			tmp.tokens[i] = token_array.tokens[i];
			i++;
		}
		while (i < token_array.count) // free the rest of the tokens
		{
			delete token_array.tokens[i].contents;
			i++;
		}
		delete token_array.tokens;
		token_array = tmp;
	}
	
	//DeleteTokens(token_array);
	//token_array.tokens = new Token[size];
	//token_array.capacity = size;
}

bool IsWhiteSpace(char c)
{
	bool result = (c == ' ') || (c == '\t') || (c == '\f') || (c == '\v');
	return result;
}

bool IsCRLF(Tokenizer &tokenizer)
{
	bool result = (tokenizer.string[tokenizer.position] == '\r') && (tokenizer.string[tokenizer.position + 1] == '\r');
	return result;
}

bool IsEndOfLine(char c)
{
	bool result = (c == '\n') || (c == '\r');
	return result;
}

bool IsLetter(char c)
{
	bool result = false;

	if ((c >= 'A') && (c <= 'Z')) result = true;
	if ((c >= 'a') && (c <= 'z')) result = true;

	return result;
}

bool IsNumeric(char c)
{
	bool result = false;

	if ((c >= '0') && (c <= '9')) result = true;

	return result;
}

char* TokenTypeToString(TokenType type)
{
	switch (type)
	{
	case TokenType_IDENTIFIER:
		return "Identifier";
	case TokenType_DIGIT:
	case TokenType_FLOAT:
	case TokenType_STRING:
	case TokenType_BOOL:
		return "Literal";
	case TokenType_PLUS:
	case TokenType_MINUS:
	case TokenType_TIMES:
	case TokenType_SLASH:
	case TokenType_VBAR:
	case TokenType_AMP:
	case TokenType_PERIOD:
	case TokenType_EQL:
	case TokenType_NOT:
	case TokenType_LSS:
	case TokenType_GTR:
	case TokenType_LEQ:
	case TokenType_GEQ:
		return "Operator";
	case TokenType_LPAREN:
	case TokenType_RPAREN:
	case TokenType_LBRACK:
	case TokenType_RBRACK:
	case TokenType_LBRACE:
	case TokenType_RBRACE:
	case TokenType_COLON:
	case TokenType_SEMICOLON:
	case TokenType_COMMA:
	case TokenType_BECOMES:
		return "Seperator";
	case TokenType_IF:
	case TokenType_WHILE:
	case TokenType_FOR:
	case TokenType_RETURN:
	case TokenType_DO:
	case TokenType_NEW:
	case TokenType_DELETE:
	case TokenType_NULL:
		return "Keyword";
	case TokenType_EOF:
		return "EndOfStream";
	case TokenType_UNKNOWN:
	default: return "unknown";
	}
}

//char* TokenToString(Token token)
//{
//
//}