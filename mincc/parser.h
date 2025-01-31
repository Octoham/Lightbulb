#pragma once

#include "ast.h"
#include "token.h"

#include <vector>
#include <iostream>

class Parser
{
private:
    // The tokens to be parsed
    std::vector<Token> tokens;

    // The index of the next token (in a zero-indexed array)
    int tokenIndex;

    // The current token
    Token currentToken;

    // The root of the AST
    Node* root;

    // Function to advance to the next token
    void advance();

    // Function to check if the current token matches a specific type
    bool match(TokenType type);

    // Function to check if the current token matches a specific value
    bool match(std::string value);

    // Function to parse a factor (e.g., identifier, literal)
    Node* parseFactor();

    // Function to parse a term (e.g., factor, binary operation)
    Node* parseTerm();

    // Function to parse an expression (e.g., term, binary operation)
    Node* parseExpression();

    // Function to parse a statement (e.g., expression, return, block)
    Node* parseStatement();

    // Function to parse a block (e.g., multiple statements)
    Node* parseBlock();

    // Function to parse a function declaration
    Node* parseFunctionDeclaration();

    // Function to parse a variable declaration
    Node* parseVariableDeclaration();

public:
    // Constructor to initialize the parser with tokens
    Parser(std::vector<Token> tokens);

    // Function to parse the tokens into an AST
    Node* parse();

    // Function to parse the AST recieved
    void printAST(Node* node, int indent = 0);

    // Function to parse the AST contained in the Parser
    void printAST();
};
