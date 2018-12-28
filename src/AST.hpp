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
using std::shared_ptr
using std::cout;
using std::endl;
using llvm::Value;

class scNNode;
class scNType;
class scNParams;
class scNDeclarationBody;
class scNStatements;
class scNStatement;
class scNReturnStatement
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

class scNExpression: public scNStatement {
public:
	bool is_assignable;
	shared_ptr<scType> type;
public:
	scNExpression(): class_name("scNExpression"),
					 is_assignable(false),
					 type(nullptr) {}
	~scNExpression() {}

	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
	}

	virtual llvm::Value* code_generate(scContext& context) {}
};

class scNIdentifier: public scNExpression {
public:
	string name;
public:
	scIdentifier(string name): class_name("scNIdentifier"),
							   name(name) {}
	~scIdentifier() {}
	
	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
		this->print_depth(depth+1);
		cout<<this->name<<endl;
	}

	llvm::Value* code_generate(scContext& context) {}
};

class scNFunctionCall: public scNExpression {
public:
	string f_name;
	shared_ptr<scNExpressions> expressions;
public:
	scNFunctionCall(string name, shared_ptr<scNExpressions> expressions):
					class_name("scNFunctionCall"),
					f_name(name),
					expressions(expressions) {}
	~scNFunctionCall() {}
	
	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
		this->print_depth(depth+1);
		cout<<this->f_name<<endl;
		this->expressions->print_debug(depth+1);
	}

	llvm::Value* code_generate(scContext& context) {}
};

class scNExpressions: public scNExpression {
public:
	vector<shared_ptr<scNExpression> > expression_list;
public:
	scNExpressions(): class_name("scNExpressions") {}
	~scNExpressions() {}
	
	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
		for(auto it = expression_list.begin(); it!=expression_list.end(); ++it) {
			(*it)->print_debug(depth+1);
		}
	}

	llvm::Value* code_generate(scContext& context) {}
};

class scNArrayExpression: public scNExpression {
public:
	shared_ptr<scNExpression> target_expression;
	shared_ptr<scNExpression> index_expression;
public:
	scNArrayExpression(shared_ptr<scNExpression> target_expression, 
					   shared_ptr<scNExpression> index_expression):
					   class_name("scNArrayExpression"),
					   target_expression(target_expression),
					   index_expression(index_expression) {}
	~scNArrayExpression() {}
	
	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
		target_expression->print_debug(depth+1);
		index_expression->print_debug(depth+1);
	}

	llvm::Value* code_generate(scContext& context) {}
};

class scNString: public scNExpression {
public:
	string value;
public:
	scNString(string value): class_name("scNString"),
							value(value) {}
	~scNString() {}

	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
		this->print_depth(depth+1);
		cout<<this->value<<endl;
	}

	llvm::Value* code_generate(scContext& context) {}
};

class scNChar: public scNExpression {
public:
	string value;
public:
	scNChar(string value): class_name("scNChar"),
						   value(value) {}
	~scNChar() {}

	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
		this->print_depth(depth+1);
		cout<<this->value<<endl;
	}

	llvm::Value* code_generate(scContext& context) {}
};

class scNNumber: public scNExpression {
public:
	scNNumber(): class_name("scNNumber") {}
	~scNNunmber() {}

	virtual llvm::Value* code_generate(scContext& context) {}
};

class scNInt32Number: public scNExpression {
public:
	int value;
public:
	scNInt32Number(int value): class_name("scNInt32Number"),
							   value(value) {}
	~scNInt32Number() {}

	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
		this->print_depth(depth+1);
		cout<<this->value<<endl;
	}

	llvm::Value* code_generate(scContext& context) {}
}

class scNDouble64Number: public scNExpression {
public:
	double value;
public:
	scNDouble64Number(double value): class_name("scNDouble64Number"),
									 value(value) {}
	~scNDouble64Number() {}

	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
		this->print_depth(depth+1);
		cout<<this->value<<endl;
	}

	llvm::Value* code_generate(scContext& context) {}
}

class scNAssignment: public scNExpression {
public:
	shared_ptr<scNExpression> left_expression;
	shared_ptr<scNExpression> right_expression;
public:
	scNAssignment(shared_ptr<scNExpression> left_expression,
				  shared_ptr<scNExpression> right_expression):
				  left_expression(left_expression),
				  right_expression(right_expression) {}
	~scNAssignment() {}

	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
		left_expression->print_debug(depth+1);
		right_expression->print_debug(depth+1);
	}

	llvm::Value* code_generate(scContext& context) {}
}

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
						b_op(b_op) {}
	~scNBinaryExpression() {}

	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
		left_expression->print_debug(depth+1);
		this->print_depth(depth+1);
		cout<<this->b_op<<endl;
		right_expression->print_debug(depth+1);
	}

	llvm::Value* code_generate(scContext& context) {}
}

class scNUnaryExpression: public scNExpression {
public:
	shared_ptr<scNExpression> expression;
	int u_op;
public:
	scNUnaryExpression(shared_ptr<scNExpression> expression, int u_op):
					  expression(expression), u_op(u_op) {}
	~scNUnaryExpression() {}

	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
		this->print_depth(depth+1);
		cout<<this->u_op<<endl;
		expression->print_debug(depth+1);
	}

	llvm::Value* code_generate(scContext& context) {}
}

class scNReferenceExpression: public scNExpression {
public:
	shared_ptr<scNExpression> expression;
public:
	scNReferenceExpression(shared_ptr<scNExpression> expression):
						   expression(expression) {}
	~scNReferenceExpression() {}

	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
		this->print_depth(depth+1);
		cout<<"&"<<endl;
		expression->print_debug(depth+1);
	}

	llvm::Value* code_generate(scContext& context) {}
}

class scNDereferenceExpression: public scNExpression {
public:
	shared_ptr<scNExpression> expression;
public:
	scNDereferenceExpression(shared_ptr<scNExpression> expression):
							 expression(expression) {}
	~scNDereferenceExpression() {}

	void print_debug(int depth) {
		this->print_depth(depth);
		cout<<this->class_name<<endl;
		this->print_depth(depth+1);
		cout<<"*"<<endl;
		expression->print_debug(depth+1);
	}

	llvm::Value* code_generate(scContext& context) {}
}

class scNNode
{
public:
    string class_name;
public:
    scNNode():class_name("scNNode"){}
    void print_debug(int depth)
    {
        cout<<this->class_name<<endl;
    }
    static void print_depth(int depth)
    {
        for(int i=0; i<depth; i++)
            cout<<'--';
    }
    virtual Value* code_generate(scContext& context);
};

class scNType : public scNNode
{
public:
    int type;
    int count;
public:
    scNType(int type):type(type), class_name("scNType"){}
    Value* code_generate(scContext& context){}
    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name<<endl;
        this->print_depth(depth+1);
        cout<<this->type<<endl<<this->count<<endl;
    }
};

class scNStatement : public scNNode
{
public:
    scNStatement():class_name("scNStatement"){}
    virtual Value* code_generate(scContext& context);
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
    scNDeclarationBody(){}
    Value* code_generate(scContext& context){}
    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name;
        this->print_depth(depth+1);
        cout<<this->is_array<<endl
        <<this->is_ptr<<endl
        <<this->size<<endl
        <<this->p_size<<endl
        <<this->name<<endl;

        this->children->print_debug(depth+1);
    }
};

class scNDeclaration : public scNStatement
{
public:
    scNDeclaration() : class_name("scNDeclaration"){}
    virtual Value* code_generate(scContext& context);
};

class scNStatements : public scNStatement
{
public:
    vector<shared_ptr<scNStatement>> statement_list;
public:
    scNStatements(){}
    Value* code_generate(scContext& context){}

    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name<<endl;
        for(auto it = this->statement_list.begin(); it != this->statement_list.end(); it++)
        {
            (*it)->print_debug(depth+1);
        }
    }
};

class scNEmptyStatement : public scNStatement
{
public:
    scNEmptyStatement(){}
    Value* code_generate(scContext& context){}
    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name;
    }
};

class scNBlock : public scNStatement
{
public:
    shared_ptr<scNStatements> statements;

public:
    scNBlock(){}

    Value* code_generate(scContext& context){}
    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name;
        statements->print_debug(depth+1);
    }
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
    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name;
        this->expression->print_debug(depth+1);
        this->statement->print_debug(depth+1);
    }
};

class scNIfElseStatement : public scNStatement
{
public:
    shared_ptr<scNExpression> expression;
    shared_ptr<scNStatement> if_statement;
    shared_ptr<scNStatement> else_statement;

public:
    scNIfElseStatement(shared_ptr<scNExpression> ex, shared_ptr<scNStatement> i_st, shared_ptr<scNStatement> e_st)
    : expression(ex), if_statement(i_st), else_statement(e_st){}

    Value* code_generate(scContext& context){}

    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name;
        this->expression->print_debug(depth+1);
        this->if_statement->print_debug(depth+1);
        this->else_statement->print_debug(depth+1);
    }
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
    : init_expression(i_ex), cond_expression(c_ex), update_expression(u_ex), statement(st){}

    Value* code_generate(scContext& context){}

    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name;

        this->init_expression->print_debug(depth+1);
        this->cond_expression->print_debug(depth+1);
        this->update_expression->print_debug(depth+1);
        this->statement->print_debug(depth+1);
    }
};

class scNWhileStatement : public scNStatement
{
public:
    shared_ptr<scNExpression> expression;
    shared_ptr<scNStatement> statement;
public:
    scNWhileStatement(shared_ptr<scNExpression> ex, shared_ptr<scNStatement> st)
    : expression(ex), statement(st){}

    Value* code_generate(scContext& context){}

    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name;
        this->expression->print_debug(depth+1);
        this->statement->print_debug(depth+1);
    }
};

class scNFunctionDefinition : public scNStatement
{
public:
    shared_ptr<scNFunctionDeclaration> func_declaration;
    shared_ptr<scNBlock> block;
public:
    scNFunctionDefinition(shared_ptr<scNFunctionDeclaration> dec, hared_ptr<scNBlock> blk)
    : func_declaration(dec), block(blk){}

    Value* code_generate(scContext& context){}

    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name;
        this->func_declaration->print_debug(depth+1);
        this->block->print_debug(depth+1);
    }
};

class scNReturnStatement : public scNStatement
{
public:
    shared_ptr<scNExpression> expression;
public:
    scNReturnStatement(shared_ptr<scNExpression> ex)
    : expression(ex){}
    Value* code_generate(scContext& context){}

    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name;
        this->expression->print_debug(depth+1);
    }
};


class scNVariableDeclaration : public scNDeclaration
{
public:
    int type;
    shared_ptr<scNDeclarationBody> dec_body;
public:
    scNVariableDeclaration(int type, shared_ptr<scNDeclarationBody> body) 
    : type(type), dec_body(body) {}

    Value* code_generate(scContext& context){}

    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name;
        cout<<print_depth(depth+1);
        this->dec_body->print_debug(depth+1);
    }
};

class scNFunctionDeclaration : public scNDeclaration
{
public:
    shared_ptr<scNType> type;
    shared_ptr<scNFunctionIdentifier> func_name;
    shared_ptr<scNParams> param_list;
public:
    scNFunctionDeclaration(shared_ptr<scNType> type, string func_name, shared_ptr<scNParams> param_list)
    : type(type), func_name(func_name), param_list(param_list) {}

    Value* code_generate(scContext& context){}

    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name;
        this->type->print_debug(depth+1);
        this->func_name->print_debug(depth+1);
        this->param_list->print_debug(depth+1);
    }
};

class scNParams : public scNNode
{
public:
    vector<shared_ptr<scNVariableDeclaration>> param_list;
public:
    scNParams(){}
    Value* code_generate(scContext& context){}
    void print_debug(int depth)
    {
        this->print_depth(depth);
        cout<<this->class_name;
        for(auto it=this->param_list.begin(); it != this->param_list.end(); it++)
        {
            (*it)->print_debug(depth+1);
        }
    }
};

#endif _AST_HPP_