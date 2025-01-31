#include "parser.h"

// Constructor for the Parser class
Parser::Parser(std::vector<Token> tokens) : tokens(tokens), tokenIndex(0)
{
    // Advance to the first token
    advance();
}

// Function to advance to the next token
void Parser::advance()
{
    // Check if we've reached the end of the token list
    if (tokenIndex < tokens.size())
    {
        // Move to the next token
        tokenIndex++;
    }
    // Literally do nothing if we're already at the end
}

// Function to check if the current token matches a specific type
bool Parser::match(TokenType type)
{
    // Return true if the current token matches the specified type
    return tokens[tokenIndex].type == type;
}

// Function to check if the current token matches a specific value
bool Parser::match(std::string value)
{
    // Return true if the current token matches the specified value
    return tokens[tokenIndex].value == value;
}

// Function to parse the entire token list
Node* Parser::parse()
{
    RootNode* root = new RootNode({});

    // start parsing here

    treeRoot = root;
    return root;
}


// Function to print the Abstract Syntax Tree (AST)
void Parser::printAST(Node* node, int indent)
{
    // Check if the node is null
    if (node == nullptr)
    {
        return;
    }

    // Print the indentation
    for (int i = 0; i < indent; i++)
    {
        std::cout << "  ";
    }

    // Check if the node is a LiteralNode
    if (auto literal = dynamic_cast<LiteralNode*>(node))
    {
        // Print the literal value
        std::cout << "Literal: " << literal->value << std::endl;
    }
    // Check if the node is an IdentifierNode
    else if (auto identifier = dynamic_cast<IdentifierNode*>(node))
    {
        // Print the identifier name
        std::cout << "Identifier: " << identifier->name << std::endl;
    }
    // Check if the node is a BinaryOpNode
    else if (auto binaryOp = dynamic_cast<BinaryOpNode*>(node))
    {
        // Print the binary operation
        std::cout << "Binary Operation: " << binaryOp->op << std::endl;
        // Recursively print the left and right operands
        printAST(binaryOp->left, indent + 1);
        printAST(binaryOp->right, indent + 1);
    }
    // Check if the node is a UnaryOpNode
    else if (auto unaryOp = dynamic_cast<UnaryOpNode*>(node))
    {
        // Print the unary operation
        std::cout << "Unary Operation: " << unaryOp->op << std::endl;
        // Recursively print the operand
        printAST(unaryOp->expr, indent + 1);
    }
    // Check if the node is a VarDeclNode
    else if (auto varDecl = dynamic_cast<VarDeclNode*>(node))
    {
        // Print the variable declaration
        std::cout << "Variable Declaration: " << varDecl->type << " " << varDecl->id->name << std::endl;
    }
    // Check if the node is a FuncDeclNode
    else if (auto funcDecl = dynamic_cast<FuncDeclNode*>(node))
    {
        // Print the function declaration
        std::cout << "Function Declaration: " << funcDecl->returnType << " " << funcDecl->id->name << std::endl;
        // Recursively print the parameters
        for (auto param : funcDecl->params)
        {
            printAST(param, indent + 1);
        }
        // Recursively print the block
        std::cout << "Function Block:" << std::endl;
        printAST(funcDecl->block, indent + 1);
    }
    // Check if the node is a ReturnNode
    else if (auto returnStmt = dynamic_cast<ReturnNode*>(node))
    {
        // Print the return statement
        std::cout << "Return Statement" << std::endl;
        // Recursively print the expression
        printAST(returnStmt->expr, indent + 1);
    }
    // Check if the node is an ExprStmtNode
    else if (auto exprStmt = dynamic_cast<ExprStmtNode*>(node))
    {
        // Print the expression statement
        std::cout << "Expression Statement" << std::endl;
        // Recursively print the expression
        printAST(exprStmt->expr, indent + 1);
    }
    // Check if the node is a FuncCallNode
    else if (auto funcCall = dynamic_cast<FuncCallNode*>(node))
    {
        // Print the function call
        std::cout << "Function Call: " << funcCall->id->name << std::endl;
        for (auto arg : funcCall->args)
        {
            // Recursively print the function call
            printAST(arg, indent + 1);
        }
    }
    // Check if the node is a BlockStmtNode
    else if (auto blockStmt = dynamic_cast<BlockStmtNode*>(node))
    {
        // Print the block statement
        std::cout << "Block Statement" << std::endl;
        // Recursively print the statements
        for (auto stmt : blockStmt->stmts)
        {
            printAST(stmt, indent + 1);
        }
    }
    // Check if the node is a RootNode
    else if (auto root = dynamic_cast<RootNode*>(node))
    {
        // Print the root node
        std::cout << "Root" << std::endl;
        // Recursively print the statements
        for (auto cnt : root->cnts)
        {
            printAST(cnt, indent + 1);
        }
    }
}

// Function to print the Abstract Syntax Tree (AST)
void Parser::printAST()
{
    // Parse the token list
    Node* root = parse();
    // Print the AST
    printAST(root);
}
