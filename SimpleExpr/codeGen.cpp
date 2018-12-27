#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <iostream>
#include <string>
#include <vector>


// #include "context.hpp"
#include "otherContext.h"
#include "AST.hpp"
#include "grammar.hpp"

using std::cout;
using std::endl;

llvm::Value* NExprs::codeGen(CodeGenContext &context) {
	Value* last;
	int i = 0;
	for(std::vector<NExpr*>::iterator it = exprs.begin(); it!=exprs.end(); ++it)
	{
		cout << "genCode: " << ++i << endl;
		last = (*it)->codeGen(context);
	}
	return last;
}

llvm::Value* Symbol::codeGen(CodeGenContext &context) {
	cout << "searching symbol" << name << endl;
	Value* value = context.getSymbolValue(this->name);
	cout << "searching symbol done" << endl;
	return context.builder.CreateLoad(value, false, "");
}

llvm::Value* Bop::codeGen(CodeGenContext &context) {
	cout << "lc: " << endl;
	Value* lc = lexpr->codeGen(context);
	cout << "rc: " << endl;
	Value* rc = rexpr->codeGen(context);
	switch(op) {
		case TPLUS:
			cout << "TPLUS-GEN: " << endl;
			return context.builder.CreateAdd(lc, rc, "addtmp");
		case TMINUS:
			cout << "TMINUS-GEN: " << endl;
			return context.builder.CreateSub(lc, rc, "subtmp");
		default:
			cout << "nullptr: " << endl;
			return nullptr;
	}
}

llvm::Value* Assign::codeGen(CodeGenContext &context) {
	Value* dst = context.getSymbolValue(symbol->name);
	Value* expv = expr->codeGen(context);
	if(!dst) {
		cout << "dst = nullptr" << endl;
		llvm::Type* type = llvm::Type::getInt32Ty(context.llvmContext);
		cout << "Assign1" << endl;
		dst = context.builder.CreateAlloca(type);
		cout << "Assign2" << endl;
		// context.saveSymbolValue(symbol->name, dst);
		context.setSymbolValue(symbol->name, dst);
	}
	cout << "Assign3" << endl;
	context.builder.CreateStore(expv, dst);
	cout << "Assign before return" << endl;

	// Value* dst = context.saveSymbolValue(symbol->name, expv);
	return dst;
}

llvm::Value* Number::codeGen(CodeGenContext &context) {
	return ConstantFP::get(Type::getDoubleTy(context.llvmContext), this->value);
}

// void CodeGenContext::generateCode(NExprs* root) {
//     cout << "Generating IR code" << endl;
//     std::vector<Type*> sysArgs;
//     llvm::FunctionType* mainFuncType = llvm::FunctionType::get(llvm::Type::getVoidTy(this->llvmContext), makeArrayRef(sysArgs), false);
//     llvm::Function* mainFunc = llvm::Function::Create(mainFuncType, llvm::GlobalValue::ExternalLinkage, "main");
// 	BB = BasicBlock::Create(llvmContext, "entry");
// 	builder.SetInsertPoint(BB);
// 	root->codeGen(*this);
// }
