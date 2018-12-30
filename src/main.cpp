#include "AST.hpp"
#include "context.h"
#include <iostream>
#include <fstream>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Function.h"
#include <llvm/IR/LegacyPassManager.h>
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Support/raw_os_ostream.h"

using namespace std;

extern int yyparse();
extern scNStatements* TopBlock;

void writeModuleToFile(llvm::Module* module, string filename)
{
	std::ofstream std_file_stream(filename);
	llvm::raw_os_ostream file_stream(std_file_stream);
	assert(module != nullptr);
	module->print(file_stream, nullptr);
}

int main() {
	yyparse();
	cout<<"Finish parsing"<<endl;

	TopBlock->print_debug(0);
	
	scContext context;
	std::vector<Type*> sysArgs;
    llvm::FunctionType* funcType = llvm::FunctionType::get(Type::getVoidTy(context.llvmContext), makeArrayRef(sysArgs), false);
    llvm::Function *mainfunc = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, "main");
	BasicBlock* basicBlock = BasicBlock::Create(context.llvmContext, "top-entry");

	context.pushBlock(basicBlock);
	TopBlock->code_generate(context);
	context.popBlock();

	cout<<"Finish code generate"<<endl;

    // llvm::legacy::PassManager passManager;
    // passManager.add(createPrintModulePass(llvm::outs()));
    // passManager.run(*(context.llvmModule.get()));

	string filename = "out.ll";
	writeModuleToFile(context.llvmModule.get(), filename);
	return 0;
}