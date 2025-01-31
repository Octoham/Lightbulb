#pragma once

#include <vector>
#include <string>

// Enum representing the data types supported by the compiler
enum VarType
{
    // 32-bit integer type
    Type_Int32
};

// Base node class for the Abstract Syntax Tree (AST)
class Node
{
public:
    // Virtual destructor to ensure proper cleanup of derived classes
    virtual ~Node() = default;
};

class RootNode : public Node
{
public:
    // Constructor to initialize the statements in the block
    RootNode(std::vector<Node*> cnts) : cnts(cnts) {}

    // The contents of the RootNode
    std::vector<Node*> cnts;
};

// Node representing a block statement (e.g., "{ x = 5; y = 10; }")
class BlockStmtNode : public Node
{
public:
    // Constructor to initialize the statements in the block
    BlockStmtNode(std::vector<Node*> stmts) : stmts(stmts) {}

    // The statements in the block
    std::vector<Node*> stmts;
};

// Node representing a literal value (e.g., integer constant)
class LiteralNode : public Node
{
public:
    // Constructor to initialize the literal value
    LiteralNode(int value) : value(value) {}

    // The literal value itself
    int value;
};

// Node representing an identifier (e.g., variable or function name)
class IdentifierNode : public Node
{
public:
    // Constructor to initialize the identifier name
    IdentifierNode(std::string name) : name(name) {}

    // The identifier name itself
    std::string name;
};

// Node representing a binary operation (e.g., addition, subtraction)
class BinaryOpNode : public Node
{
public:
    // Constructor to initialize the left operand, operator, and right operand
    BinaryOpNode(Node* left, std::string op, Node* right)
        : left(left), op(op), right(right) {}

    // The left operand of the binary operation
    Node* left;

    // The operator itself (e.g., "+", "-", etc.)
    std::string op;

    // The right operand of the binary operation
    Node* right;
};

// Node representing a unary operation (e.g., negation, increment)
class UnaryOpNode : public Node
{
public:
    // Constructor to initialize the operator and operand
    UnaryOpNode(std::string op, Node* expr)
        : op(op), expr(expr) {}

    // The operator itself (e.g., "-", "++", etc.)
    std::string op;

    // The operand of the unary operation
    Node* expr;
};

// Node representing a variable declaration (e.g., "int x;")
class VarDeclNode : public Node
{
public:
    // Constructor to initialize the variable type and identifier
    VarDeclNode(VarType type, IdentifierNode* id)
        : type(type), id(id) {}

    // The data type of the variable
    VarType type;

    // The identifier of the variable
    IdentifierNode* id;
};

// Node representing a function declaration (e.g., "int foo(int x, int y);")
class FuncDeclNode : public Node
{
public:
    // Constructor to initialize the return type, function name, and parameters
    FuncDeclNode(VarType returnType, IdentifierNode* id, std::vector<VarDeclNode*>)
        : returnType(returnType), id(id), params(params) {}

    // The return type of the function
    VarType returnType;

    // The identifier of the function
    IdentifierNode* id;

    // The parameters of the function
    std::vector<VarDeclNode*> params;
};

// Node representing a function definition (e.g., "int foo(int x, int y) { return x + y; } ")
class FuncDefNode : public Node
{
public:
    // Constructor to initialize the head and the body
    FuncDefNode(FuncDeclNode* head, BlockStmtNode* body) : head(head), body(body) {}

    // The head of the function
    FuncDeclNode* head;

    // The body of the function
    BlockStmtNode* body;
};

// Node representing a return statement (e.g., "return x;")
class ReturnNode : public Node
{
public:
    // Constructor to initialize the return expression
    ReturnNode(Node* expr) : expr(expr) {}

    // The expression being returned
    Node* expr;
};

// Node representing an expression statement (e.g., "x = 5;")
class ExprStmtNode : public Node
{
public:
    // Constructor to initialize the expression
    ExprStmtNode(Node* expr) : expr(expr) {}

    // The expression itself
    Node* expr;
};

// Node representing a function call (e.g., "foo(x, y)")
class FuncCallNode : public Node
{
public:
    // Constructor to initialize the function name and arguments
    FuncCallNode(IdentifierNode* id, std::vector<Node*> args)
        : id(id), args(args) {}

    // The identifier of the function
    IdentifierNode* id;

    // The arguments of the function call
    std::vector<Node*> args;
};
