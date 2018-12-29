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
	module->print(file_stream, nullptr);
}

int main() {
	yyparse();
	cout<<"Finish parsing"<<endl;
	TopBlock->print_debug(0);
	scContext context;

    llvm::FunctionType* funcType = llvm::FunctionType::get(context.builder.getInt32Ty(), false);
    llvm::Function *mainfunc = llvm::Function::Create(funcType, Function::ExternalLinkage, "main", context.llvmModule.get());
	BasicBlock* basicBlock = BasicBlock::Create(context.llvmContext, "top-entry", mainfunc, nullptr);
	context.builder.SetInsertPoint(basicBlock);

	context.pushBlock(basicBlock);

    context.builder.CreateGlobalStringPtr("fuc");
	TopBlock->code_generate(context);

	cout<<"topblock done"<<endl;

	context.popBlock();
	string filename = "out.ll";
	writeModuleToFile(context.llvmModule.get(), filename);
	return 0;
}