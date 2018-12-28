#include <cstdio>
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

using namespace llvm;

static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);

std::unique_ptr<Module> buildModule()
{
	std::unique_ptr<Module> module = llvm::make_unique<Module>("top", TheContext);

	/* Create main function */
	FunctionType *funcType = FunctionType::get(Builder.getInt32Ty(), false);	
	Function *mainFunc = Function::Create(funcType, Function::ExternalLinkage, "main", module.get());
	BasicBlock *entry = BasicBlock::Create(TheContext, "entrypoint", mainFunc);
	Builder.SetInsertPoint(entry);

	/* String constant */
	Value *helloWorldStr = Builder.CreateGlobalStringPtr("hello world!");
	Value *formatStr = Builder.CreateGlobalStringPtr("%d\n");
	Type* intType = Builder.getInt32Ty();
	Value *var = Builder.CreateAlloca(intType);
	Value *cst = ConstantInt::get(TheContext, APInt(32, 999));
	Builder.CreateStore(cst, var);

	/* Create "puts" function */
	std::vector<Type *> putsArgs;
	putsArgs.push_back(Builder.getInt8Ty()->getPointerTo());
	ArrayRef<Type*> putsargsRef(putsArgs);
	FunctionType *putsType = FunctionType::get(Builder.getInt32Ty(), putsargsRef, false);
	Constant *putsFunc = module->getOrInsertFunction("puts", putsType);

	/* Create "printf" function */
	std::vector<Type *> printfArgs;
	printfArgs.push_back(Builder.getInt8Ty()->getPointerTo());
	ArrayRef<Type*> printfargsRef(printfArgs);
	FunctionType *printfType = FunctionType::get(Builder.getInt32Ty(), printfargsRef, true);
	Constant *printfFunc = module->getOrInsertFunction("printf", printfType);

	/* Invoke it */
	Builder.CreateCall(putsFunc, helloWorldStr);

	/* Invoke it */
	std::vector<Value*> argv;
	argv.push_back(formatStr);
	argv.push_back(Builder.CreateLoad(var));
	Builder.CreateCall(printfFunc, argv);

	/* Return zero */
	Builder.CreateRet(ConstantInt::get(TheContext, APInt(32, 0)));

	return module;
}

void writeModuleToFile(Module *module)
{
	std::ofstream std_file_stream("out.ll");
	raw_os_ostream file_stream(std_file_stream);
	module->print(file_stream, nullptr);
}

int main()
{
	std::unique_ptr<Module> module = buildModule();
	writeModuleToFile(module.get());
	return 0;
}