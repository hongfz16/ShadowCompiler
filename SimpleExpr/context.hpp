#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Type.h>

#include <map>
#include <vector>
#include <string>

class NExprs;

using std::string;
using std::map;
using std::cout;
using std::endl;
using llvm::Value;
using llvm::IRBuilder;
using llvm::Module;
using llvm::LLVMContext;
using llvm::BasicBlock;
using llvm::Type;

typedef map<string, Value*> mSymbolTable;
class CodeGenContext {
public:
	IRBuilder<> builder;
	mSymbolTable symboltable;
	LLVMContext llvmContext;
	std::unique_ptr<Module> module;
	llvm::Type* doubleTy;
	BasicBlock *BB;

	CodeGenContext():builder(llvmContext) {
		module = std::unique_ptr<Module>(new Module("main", this->llvmContext));
		doubleTy = llvm::Type::getInt32Ty(llvmContext);
	}

	Value* getSymbolValue(string name) {
		auto res = symboltable.find(name); 
		return res == symboltable.end()? nullptr : res->second;
	}

	void saveSymbolValue(string name, Value* value) {
		symboltable[name] = value;
	}

	void generateCode(NExprs* root);
};

#endif //_CONTEXT_H_