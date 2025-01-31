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

    // The index of the current token (in a zero-indexed array)
    int tokenIndex;

    // The root of the AST
    Node* treeRoot;

    // Function to advance to the next token
    void advance();

    // Function to check if the current token matches a specific type
    bool match(TokenType type);

    // Function to check if the current token matches a specific value
    bool match(std::string value);

    // Function to parse the next statement
    Node* parseStatement();

    // Function for errors lmao
    void PrintError(std::string err);

public:
    // Constructor to initialize the parser with tokens
    Parser(std::vector<Token> tokens);

    // Function to parse the tokens into an AST
    Node* parse();
    
    // Function to parse the AST recieved
    void printAST(Node* node, int indent = 0);

    // Function to parse the AST contained in the Parser
    void printAST();

    // Function to get the VarType of a string
    VarType stringToVarType(std::string str);

};
