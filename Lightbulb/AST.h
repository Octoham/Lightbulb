#pragma once

enum Type
{
	// integers
	Type_Int8, // 
	Type_UInt8, // 
	Type_Int16, // short
	Type_UInt16, // ushort
	Type_Int32, // int
	Type_UInt32, // uint
	Type_Int64, // long
	Type_UInt64, // ulong
	Type_Int128, //
	Type_UInt128, //

	// floating points
	Type_Half, // half
	Type_Float, // float
	Type_Double, // double
	Type_Quad, // quad

	// pointers
	Type_Pointer,

	// void
	Type_Void,

	// bool
	Type_Bool


};

class ExprAST
{
public:
	virtual ~ExprAST() = default;
};

class BodyExprAST : public ExprAST // the shit that functions have and code blocks have
{
public:
	ExprAST** contents; // array of pointers to all the other ExprAST
	int length; // length of the array
	BodyExprAST(ExprAST** contents, int length) : contents(contents), length(length) {}
};

class LiteralExprAST : public ExprAST // the shit that literal values have
{
public:
	void* contents; // pointer to contents of the literal value
	int size; // size of the contents in bytes
	Type type; // type of the contents
	LiteralExprAST(void* contents, int size, Type type) : contents(contents), size(size), type(type) {}
};

class VarDeclExprAST : public ExprAST // the shit that declares a variable
{
public:
	char* name; // variable name
	int nlength; // variable name length
	Type type; // variable type
	VarDeclExprAST(char* name, int nlength, Type type) : name(name), nlength(nlength), type(type) {}
};

class FuncDeclExprAST : public ExprAST // the shit that declares a function, essentially the function prototype
{
public:
	char* name; // function name
	int nlength; // function name length
	VarDeclExprAST** args; // function arguments as array of variable declarations, VarDeclExprAST is used since it has everything that a function argument has
	int argc; // number of args
	Type type; // return type
	FuncDeclExprAST(char* name, int nlength, VarDeclExprAST** args, int argc, Type type) : name(name), nlength(nlength), args(args), argc(argc), type(type) {}
};

class FuncDefExprAST : public ExprAST // the shit that defines a function
{
public:
	FuncDeclExprAST* declaration; // the prototype
	BodyExprAST* body; // the actual contents
	FuncDefExprAST(FuncDeclExprAST* declaration, BodyExprAST* body) : declaration(declaration), body(body) {}
};

class VarExprAST : public ExprAST // the shit that is a variable reference (variable call?)
{
public:
	char* name; // variable name
	int nlength; // variable name length
	VarExprAST(char* name, int nlength) : name(name), nlength(nlength) {}
};

class FuncExprAST : public ExprAST // the shit that is a function call
{
public:
	char* name; // function name
	int nlength; // function name length
	ExprAST** args; // all the arguments passed on to the function
	int argc; // number of arguments
	FuncExprAST(char* name, int nlength, ExprAST** args, int argc) : name(name), nlength(nlength), args(args), argc(argc) {}
};

class OpExprAST : public ExprAST // the shit that is an operator
{
public:
	char* op; // operator
	int oplength; // operator length
	ExprAST* left; // expression on the left of the operator
	ExprAST* right; // expression on the right of the operator
	OpExprAST(char* op, int oplength, ExprAST* left, ExprAST* right) : op(op), oplength(oplength), left(left), right(right) {}
};

class PointerExprAST : public ExprAST
{

};

class ArrayLiteralExprAST : public ExprAST
{
public:

};

// todo: Arrays ig and Pointers (declaration and ref)