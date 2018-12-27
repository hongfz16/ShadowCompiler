#ifndef _AST_H_
#define _AST_H_

#include <llvm/IR/Value.h>
#include <iostream>
#include <string>
#include <vector>

// #include "context.hpp"
#include "otherContext.h"

using namespace std;
using llvm::ConstantFP;
using llvm::Type;

class Node;
class NExpr;
class NExprs;
class Number;
class Symbol;
class Bop;
class Assign;

class Node {
public:
	Node(){}
	virtual ~Node(){}
	virtual void print(){}
	virtual llvm::Value* codeGen(CodeGenContext &context) { return (llvm::Value *)0; }
};

class NExpr: public Node {
public:
	NExpr() {}
	virtual ~NExpr() {}
	virtual llvm::Value* codeGen(CodeGenContext &context) { return (llvm::Value *)0; }
};

class NExprs: public Node {
public:
	vector<NExpr*> exprs;
	NExprs() {}
	~NExprs() {}
	llvm::Value* codeGen(CodeGenContext &context);
};

class Number: public NExpr {
public:
	double value;
	Number(double value):value(value) {}
	~Number() {}
	llvm::Value* codeGen(CodeGenContext &context);
};

class Symbol: public NExpr {
public:
	bool isType;
	bool isArray;
	string name;
	Symbol(string name):name(name), isArray(false), isType(false) {}
	~Symbol() {}
	llvm::Value* codeGen(CodeGenContext &context);
};

class Bop: public NExpr {
public:
	NExpr* lexpr;
	NExpr* rexpr;
	int op;
	Bop(NExpr* lexpr, int op, NExpr* rexpr): lexpr(lexpr), op(op), rexpr(rexpr) {}
	~Bop() {}
	llvm::Value* codeGen(CodeGenContext &context);
};

class Assign: public NExpr {
public:
	Symbol* symbol;
	NExpr* expr;
	Assign(Symbol* symbol, NExpr* expr): symbol(symbol), expr(expr) {}
	~Assign() {}
	llvm::Value* codeGen(CodeGenContext &context);
};

#endif //_AST_H_