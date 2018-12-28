#ifndef _AST_HPP_
#define _AST_HPP_

#include <llvm/IR/Value.h>
#include <iostream>
#include <string>
#include <vector>
#include "context.h"

using std::string;
using std::vector;
using std::shared_ptr
using std::cout;
using std::endl;
using llvm::Value;

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

#endif //_AST_HPP_