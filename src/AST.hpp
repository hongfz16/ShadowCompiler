#ifndef _AST_HPP_
#define _AST_HPP_

#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/Support/raw_ostream.h>
#include <iostream>
#include <string>
#include <vector>
#include "context.h"

using std::string;
using llvm::Type;
using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;
using llvm::Value;

class scNNode;
class scNType;
class scNParams;
class scNDeclarationBody;
class scNStatements;
class scNStatement;
class scNReturnStatement;
class scNFunctionDeclaration;
class scNFunctionDefinition;
class scNEmptyStatement;
class scNWhileStatement;
class scNForStatement;
class scNDeclaration;
class scNVariableDeclaration;
class scNIfElseStatement;
class scNIfStatement;
class scNBlock;
class scNExpression;
class scNString;
class scNChar;
class scNNumber;
class scNInt32Number;
class scNDouble64Number;
class scNAssignment;
class scNIdentifier;
class scNBinaryExpression;
class scNFunctionCall;
class scNUnaryExpression;
class scNExpressions;
class scNReferenceExpression;
class scNArrayExpression;
class scNDereferenceExpression;

class scNNode
{
public:
    string class_name;
public:
    scNNode()
    {
        this->class_name = "scNNode";
    }
    ~scNNode() {}
    virtual void print_debug(int depth) = 0;
    static void print_depth(int depth);
    virtual Value* code_generate(scContext& context) = 0;
	void logerr(const string&);
};

class scNType : public scNNode
{
public:
    int type;
    int count;
public:
    scNType(int type):type(type)
    {
        class_name = "scNType";
    }
    Value* code_generate(scContext& context){}
    void print_debug(int depth);
};

class scNStatement : public scNNode
{
public:
    scNStatement()
    {
        class_name = "scNStatement";
    }
    virtual Value* code_generate(scContext& context) = 0;
    virtual void print_debug(int depth) = 0;
};

class scNExpression: public scNStatement {
public:
    bool is_assignable;
    shared_ptr<scType> type;
public:
    scNExpression(): is_assignable(false),
                     type(nullptr) {
                        class_name = "scNExpression";
                     }
    ~scNExpression() {}

    virtual void print_debug(int depth) = 0;

    virtual llvm::Value* code_generate(scContext& context) {}
};

class scNDeclarationBody : public scNNode
{
public:
    bool is_array;
    bool is_ptr;
    int size;
    int p_size;
    string name;
    shared_ptr<scNDeclarationBody> children;
public:
    scNDeclarationBody()
    {
        class_name = "scNDeclarationBody";
    }
    Value* code_generate(scContext& context){}
    void print_debug(int depth);
};

class scNDeclaration : public scNStatement
{
public:
    scNDeclaration()
    {
        class_name = "scNDeclaration";
    }
    virtual Value* code_generate(scContext& context) = 0;
};

class scNStatements : public scNStatement
{
public:
    vector<shared_ptr<scNStatement>> statement_list;
public:
    scNStatements()
    {
        class_name = "scNStatements";
    }
    Value* code_generate(scContext& context){}

    void print_debug(int depth);
};

class scNEmptyStatement : public scNStatement
{
public:
    scNEmptyStatement()
    {
        class_name = "scNEmptyStatement";
    }
    Value* code_generate(scContext& context){}
    void print_debug(int depth);
};

class scNBlock : public scNStatement
{
public:
    shared_ptr<scNStatements> statements;
	Function* par_func;
public:
    scNBlock()
    {
        class_name = "scNBlock";
		parent_function = nullptr;
    }

    Value* code_generate(scContext& context){}
    void print_debug(int depth);
};

class scNIfStatement : public scNStatement
{
public:
    shared_ptr<scNExpression> expression;
    shared_ptr<scNStatement> statement;

public:
    scNIfStatement(shared_ptr<scNExpression> ex, shared_ptr<scNStatement> st)
    : expression(ex), statement(st){}

    Value* code_generate(scContext& context){}
    void print_debug(int depth);
};

class scNIfElseStatement : public scNStatement
{
public:
    shared_ptr<scNExpression> expression;
    shared_ptr<scNStatement> if_statement;
    shared_ptr<scNStatement> else_statement;

public:
    scNIfElseStatement(shared_ptr<scNExpression> ex, shared_ptr<scNStatement> i_st, shared_ptr<scNStatement> e_st)
    : expression(ex), if_statement(i_st), else_statement(e_st){class_name = "scNifElseStatement";}

    Value* code_generate(scContext& context){}

    void print_debug(int depth);
};

class scNForStatement : public scNStatement
{
public:
    shared_ptr<scNExpression> init_expression;
    shared_ptr<scNExpression> cond_expression;
    shared_ptr<scNExpression> update_expression;
    shared_ptr<scNStatement> statement;
public:
    scNForStatement(shared_ptr<scNExpression> i_ex, shared_ptr<scNExpression> c_ex, shared_ptr<scNExpression> u_ex, shared_ptr<scNStatement> st)
    : init_expression(i_ex), cond_expression(c_ex), update_expression(u_ex), statement(st){ class_name = "scNForStatement"; }

    Value* code_generate(scContext& context){}

    void print_debug(int depth);
};

class scNWhileStatement : public scNStatement
{
public:
    shared_ptr<scNExpression> expression;
    shared_ptr<scNStatement> statement;
public:
    scNWhileStatement(shared_ptr<scNExpression> ex, shared_ptr<scNStatement> st)
    : expression(ex), statement(st){class_name = "scNWhileStatement";}

    Value* code_generate(scContext& context){}

    void print_debug(int depth);
};

class scNFunctionDeclaration : public scNDeclaration
{
public:
    shared_ptr<scNType> type;
    string func_name;
    shared_ptr<scNParams> param_list;
public:
    scNFunctionDeclaration(shared_ptr<scNType> type, string func_name, shared_ptr<scNParams> param_list)
    : type(type), func_name(func_name), param_list(param_list) {class_name = "scNFunctionDeclaration";}

    Value* code_generate(scContext& context){}

    void print_debug(int depth);
};

class scNFunctionDefinition : public scNStatement
{
public:
    shared_ptr<scNFunctionDeclaration> func_declaration;
    shared_ptr<scNBlock> block;
public:
    scNFunctionDefinition(shared_ptr<scNFunctionDeclaration> dec, shared_ptr<scNBlock> blk)
    : func_declaration(dec), block(blk){class_name = "scNFunctionDefinition";}

    Value* code_generate(scContext& context){}

    void print_debug(int depth);
};

class scNReturnStatement : public scNStatement
{
public:
    shared_ptr<scNExpression> expression;
public:
    scNReturnStatement(shared_ptr<scNExpression> ex)
    : expression(ex){class_name = "scNReturnStatement";}
    Value* code_generate(scContext& context){}

    void print_debug(int depth);
};


class scNVariableDeclaration : public scNDeclaration
{
public:
    int type;
    shared_ptr<scNDeclarationBody> dec_body;
public:
    scNVariableDeclaration(int type, shared_ptr<scNDeclarationBody> body) 
    : type(type), dec_body(body) {class_name = "scNVariableDeclaration";}

    Value* code_generate(scContext& context);
	scType* getTypeFromDeclarationBody(shared_ptr<scNDeclarationBody> head_ptr, bool& isarray, int& arraysize, string& name);
	void print_debug(int depth);
};

class scNParams : public scNNode
{
public:
    vector<shared_ptr<scNVariableDeclaration>> param_list;
public:
    scNParams(){class_name = "scNParams";}
    Value* code_generate(scContext& context){}
    void print_debug(int depth);
};

class scNIdentifier: public scNExpression {
public:
	string name;
public:
	scNIdentifier(string name):name(name) {
        class_name = "scNIdentifier";
    }
	~scNIdentifier() {}
	
	void print_debug(int depth);

	llvm::Value* code_generate(scContext& context) {}
};

class scNFunctionCall: public scNExpression {
public:
	string f_name;
	shared_ptr<scNExpressions> expressions;
public:
	scNFunctionCall(string name, shared_ptr<scNExpressions> expressions):
					f_name(name),
					expressions(expressions) {
                        class_name = "scNFunctionCall";
                    }
	~scNFunctionCall() {}
	
	void print_debug(int depth);

	llvm::Value* code_generate(scContext& context) {}
};

class scNExpressions: public scNExpression {
public:
	vector<shared_ptr<scNExpression> > expression_list;
public:
	scNExpressions() {
        class_name = "scNExpressions";
    }
	~scNExpressions() {}
	
	void print_debug(int depth);

	llvm::Value* code_generate(scContext& context) {}
};

class scNArrayExpression: public scNExpression {
public:
	shared_ptr<scNExpression> target_expression;
	shared_ptr<scNExpression> index_expression;
public:
	scNArrayExpression(shared_ptr<scNExpression> target_expression, 
					   shared_ptr<scNExpression> index_expression):
					   target_expression(target_expression),
					   index_expression(index_expression) {
                          class_name = "scNArrayExpression";
                       }
	~scNArrayExpression() {}
	
	void print_debug(int depth);

	llvm::Value* code_generate(scContext& context) {}
};

class scNString: public scNExpression {
public:
	string value;
public:
	scNString(string value):value(value) {
        class_name = "scNString";
    }
	~scNString() {}

	void print_debug(int depth);

	llvm::Value* code_generate(scContext& context) {}
};

class scNChar: public scNExpression {
public:
	string value;
public:
	scNChar(string value):value(value) {
        class_name = "scNChar";
    }
	~scNChar() {}

	void print_debug(int depth);

	llvm::Value* code_generate(scContext& context) {}
};

class scNNumber: public scNExpression {
public:
	scNNumber() {
        class_name = "scNNumber";
    }
	~scNNumber() {}

	virtual llvm::Value* code_generate(scContext& context) {}
};

class scNInt32Number: public scNNumber {
public:
	int value;
public:
	scNInt32Number(int value):value(value) {
        class_name = "scNInt32Number";
    }
	~scNInt32Number() {}

	void print_debug(int depth);

	llvm::Value* code_generate(scContext& context) {}
};

class scNDouble64Number: public scNNumber {
public:
	double value;
public:
	scNDouble64Number(double value): value(value) {
        class_name = "scNDouble64Number";
    }
	~scNDouble64Number() {}

	void print_debug(int depth);

	llvm::Value* code_generate(scContext& context) {}
};

class scNAssignment: public scNExpression {
public:
	shared_ptr<scNExpression> left_expression;
	shared_ptr<scNExpression> right_expression;
public:
	scNAssignment(shared_ptr<scNExpression> left_expression,
				  shared_ptr<scNExpression> right_expression):
				  left_expression(left_expression),
				  right_expression(right_expression) {
                      class_name = "scNAssignment";
                  }
	~scNAssignment() {}

	void print_debug(int depth);

	llvm::Value* code_generate(scContext& context) {}
};

class scNBinaryExpression: public scNExpression {
public:
	shared_ptr<scNExpression> left_expression;
	shared_ptr<scNExpression> right_expression;
	int b_op;
public:
	scNBinaryExpression(shared_ptr<scNExpression> left_expression,
						shared_ptr<scNExpression> right_expression,
						int b_op): 
						left_expression(left_expression),
						right_expression(right_expression),
						b_op(b_op) {
                            class_name = "scNBinaryExpression";
                        }
	~scNBinaryExpression() {}

	void print_debug(int depth);

	llvm::Value* code_generate(scContext& context) {}
};

class scNUnaryExpression: public scNExpression {
public:
	shared_ptr<scNExpression> expression;
	int u_op;
public:
	scNUnaryExpression(shared_ptr<scNExpression> expression, int u_op):
					  expression(expression), u_op(u_op) {
                          class_name="scNUnaryExpression";
                      }
	~scNUnaryExpression() {}

	void print_debug(int depth);

	llvm::Value* code_generate(scContext& context) {}
};

class scNReferenceExpression: public scNExpression {
public:
	shared_ptr<scNExpression> expression;
public:
	scNReferenceExpression(shared_ptr<scNExpression> expression):
						   expression(expression) {
                               class_name="scNReferenceExpression";
                           }
	~scNReferenceExpression() {}

	void print_debug(int depth);

	llvm::Value* code_generate(scContext& context) {}
};

class scNDereferenceExpression: public scNExpression {
public:
	shared_ptr<scNExpression> expression;
public:
	scNDereferenceExpression(shared_ptr<scNExpression> expression):
							 expression(expression) {
                                class_name="scNDereferenceExpression";
                             }
	~scNDereferenceExpression() {}

	void print_debug(int depth);

	llvm::Value* code_generate(scContext& context) {}
};


#endif //_AST_HPP_