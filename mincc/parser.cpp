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
        // Get the current token
        currentToken = tokens[tokenIndex];
        // Move to the next token
        tokenIndex++;
    }
    else
    {
        // If we've reached the end of the token list, set the current token to EOF
        currentToken.type = TOKEN_EOF;
    }
}

// Function to check if the current token matches a specific type
bool Parser::match(TokenType type)
{
    // Return true if the current token matches the specified type
    return currentToken.type == type;
}

// Function to check if the current token matches a specific value
bool Parser::match(std::string value)
{
    // Return true if the current token matches the specified value
    return currentToken.value == value;
}

// Function to parse a factor (e.g., identifier, literal)
Node* Parser::parseFactor()
{
    // Check if the current token is an identifier
    if (match(TOKEN_IDENTIFIER))
    {
        // Create a new IdentifierNode
        IdentifierNode* id = new IdentifierNode(currentToken.value);
        // Advance to the next token
        advance();
        // Return the IdentifierNode
        return id;
    }
    // Check if the current token is a literal
    else if (match(TOKEN_LITERAL))
    {
        // Create a new LiteralNode
        LiteralNode* literal = new LiteralNode(std::stoi(currentToken.value));
        // Advance to the next token
        advance();
        // Return the LiteralNode
        return literal;
    }
    else
    {
        // Handle error: unexpected token
        return nullptr;
    }
}

// Function to parse a term (e.g., factor, binary operation)
Node* Parser::parseTerm()
{
    // Parse a factor
    Node* left = parseFactor();
    // Check if the current token is a binary operator
    while (match(TOKEN_OPERATOR) && (currentToken.value == "*" || currentToken.value == "/"))
    {
        // Get the operator
        std::string op = currentToken.value;
        // Advance to the next token
        advance();
        // Parse the right operand
        Node* right = parseFactor();
        // Create a new BinaryOpNode
        left = new BinaryOpNode(left, op, right);
    }
    // Return the parsed term
    return left;
}

// Function to parse an expression (e.g., term, binary operation)
Node* Parser::parseExpression()
{
    // Parse a term
    Node* left = parseTerm();
    // Check if the current token is a binary operator
    while (match(TOKEN_OPERATOR) && (currentToken.value == "+" || currentToken.value == "-"))
    {
        // Get the operator
        std::string op = currentToken.value;
        // Advance to the next token
        advance();
        // Parse the right operand
        Node* right = parseTerm();
        // Create a new BinaryOpNode
        left = new BinaryOpNode(left, op, right);
    }
    // Return the parsed expression
    return left;
}

// Function to parse a statement (e.g., expression, return, block)
Node* Parser::parseStatement()
{
    // Check if the current token is an identifier followed by a "("
    if (match(TOKEN_IDENTIFIER) && tokens[tokenIndex].value == "(")
    {
        // Function call
        IdentifierNode* id = new IdentifierNode(currentToken.value);
        // Advance to the next token
        advance();
        // Skip the "("
        advance();
        // Parse the arguments
        std::vector<Node*> args;
        while (!match(TOKEN_SEPERATOR) || currentToken.value != ")")
        {
            // Parse an expression
            Node* arg = parseExpression();
            // Add the argument to the list
            args.push_back(arg);
            // Check if the current token is a comma
            if (match(TOKEN_SEPERATOR) && currentToken.value == ",")
            {
                // Advance to the next token
                advance();
            }
        }
        // Skip the ")"
        advance();
        // Create a new FuncCallNode
        return new FuncCallNode(id, args);
    }
    // Check if the current token is a return statement
    else if (match(TOKEN_KEYWORD) && currentToken.value == "return")
    {
        // Return statement
        // Advance to the next token
        advance();
        // Parse an expression
        Node* expr = parseExpression();
        // Create a new ReturnNode
        return new ReturnNode(expr);
    }
    // Check if the current token is a variable declaration
    else if (match(TOKEN_KEYWORD) && currentToken.value == "int")
    {
        // Variable declaration
        // Advance to the next token
        advance();
        // Parse an identifier
        IdentifierNode* id = new IdentifierNode(currentToken.value);
        // Advance to the next token
        advance();
        // Create a new VarDeclNode
        return new VarDeclNode(Type_Int32, id);
    }
    else
    {
        // Handle error: unexpected token
        return nullptr;
    }
}

// Function to parse a block (e.g., multiple statements)
Node* Parser::parseBlock()
{
    // Create a list to store the statements
    std::vector<Node*> stmts;
    // Parse statements until we reach the end of the token list
    while (!match(TOKEN_EOF))
    {
        // Parse a statement
        Node* stmt = parseStatement();
        // Add the statement to the list
        stmts.push_back(stmt);
    }
    // Create a new BlockStmtNode
    return new BlockStmtNode(stmts);
}

// Function to parse a function declaration
Node* Parser::parseFunctionDeclaration()
{
    // Function declaration
    VarType returnType = Type_Int32;
    // Advance to the next token
    advance();
    // Parse an identifier
    IdentifierNode* id = new IdentifierNode(currentToken.value);
    // Advance to the next token
    advance();
    // Skip the "("
    advance();
    // Parse the parameters
    std::vector<VarDeclNode*> params;
    while (!match(TOKEN_SEPERATOR) || currentToken.value != ")")
    {
        // Parse a variable declaration
        VarDeclNode* param = new VarDeclNode(Type_Int32, new IdentifierNode(currentToken.value));
        // Add the parameter to the list
        params.push_back(param);
        // Advance to the next token
        advance();
        // Check if the current token is a comma
        if (match(TOKEN_SEPERATOR) && currentToken.value == ",")
        {
            // Advance to the next token
            advance();
        }
    }
    // Skip the ")"
    advance();
    // Parse the block
    BlockStmtNode* block = dynamic_cast<BlockStmtNode*>(parseBlock());
    // Create a new FuncDeclNode
    return new FuncDeclNode(returnType, id, params, block);
}

// Function to parse a variable declaration
Node* Parser::parseVariableDeclaration()
{
    // Variable declaration
    VarType type = Type_Int32;
    // Advance to the next token
    advance();
    // Parse an identifier
    IdentifierNode* id = new IdentifierNode(currentToken.value);
    // Advance to the next token
    advance();
    // Create a new VarDeclNode
    return new VarDeclNode(type, id);
}

// Function to parse the entire token list
Node* Parser::parse()
{
    // Check if the current token is a function declaration
    if (match(TOKEN_KEYWORD) && currentToken.value == "int")
    {
        // Check if the next token is a "("
        if (tokens[tokenIndex + 1].value == "(")
        {
            // Parse a function declaration
            return parseFunctionDeclaration();
        }
        else
        {
            // Parse a variable declaration
            return parseVariableDeclaration();
        }
    }
    else
    {
        // Parse a block
        return parseBlock();
    }
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
}

// Function to print the Abstract Syntax Tree (AST)
void Parser::printAST()
{
    // Parse the token list
    Node* root = parse();
    // Print the AST
    printAST(root);
}
