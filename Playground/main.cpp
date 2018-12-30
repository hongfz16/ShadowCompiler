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
	Value *formatStr = Builder.CreateGlobalStringPtr("%d \n");

	Type* intType = Builder.getInt32Ty();
	Value *var = Builder.CreateAlloca(intType);
	Value *cst = ConstantInt::get(TheContext, APInt(32, 1));
	Value *cst888 = ConstantInt::get(TheContext, APInt(32, 888));
	Builder.CreateStore(cst, var);

	Value *arraysize = ConstantInt::get(TheContext, APInt(32, 200));
	Type* testtype = ArrayType::get(Builder.getInt32Ty(), 100)-> getPointerTo();
	Value* arraypointer = Builder.CreateAlloca(testtype);
	
	Value *array = Builder.CreateAlloca(ArrayType::get(Builder.getInt32Ty(), 100), arraysize);
	ArrayRef<Value*> ref = {ConstantInt::get(Type::getInt64Ty(TheContext), 0), ConstantInt::get(Type::getInt64Ty(TheContext), 100)};
	ArrayRef<Value*> ref1 = {ConstantInt::get(Type::getInt64Ty(TheContext), 1)};
	ArrayRef<Value*> refm1 = {ConstantInt::get(Type::getInt64Ty(TheContext), -1)};
	ArrayRef<Value*> ref0 = {ConstantInt::get(Type::getInt64Ty(TheContext), 0), ConstantInt::get(Type::getInt64Ty(TheContext), 0)};
	Builder.CreateStore(Builder.CreateLoad(var), Builder.CreateInBoundsGEP(array, ref, "ass100"));
	Builder.CreateStore(cst888, Builder.CreateInBoundsGEP(array, ref0, "ass0"));
	
	Builder.CreateStore(array, arraypointer);
	Constant* const1 = ConstantInt::get(Type::getInt32Ty(TheContext), 1);
	Value* loadap = Builder.CreateLoad(arraypointer);
	Value* afterap = Builder.CreateInBoundsGEP(loadap, ref1, "move");
	Value* afterafterap = Builder.CreateInBoundsGEP(afterap, refm1, "movem1");
	Builder.CreateStore(afterap, arraypointer);
	// Value* addresult = Builder.CreateAdd(Builder.CreateLoad(arraypointer), const1);
	// Builder.CreateStore(Builder.CreateLoad(addresult), arraypointer);
	// Value *pvar = Builder.CreateAlloca(intType->getPointerTo());

	// Builder.CreateStore(array, pvar);
	// Builder.CreateStore(var, pvar);
	// Value *ppvar = Builder.CreateAlloca(intType);
	// Builder.CreateStore(pvar, ppvar);

	// Value *vpvar = Builder.CreateLoad(ppvar);
	// Value *vvar = Builder.CreateLoad(vpvar);


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

	// if test

	// Value* condValue = Builder.CreateIntCast(Builder.CreateLoad(var), Type::getInt1Ty(TheContext), true);
	// Value* cond_val = Builder.CreateICmpNE(condValue, ConstantInt::get(Type::getInt1Ty(TheContext), 0, true));
	// Value* right_str = Builder.CreateGlobalStringPtr("you r right");
	// Value* wrong_str = Builder.CreateGlobalStringPtr("you r wrong");
	// Function* parent_func = Builder.GetInsertBlock()->getParent();
	// BasicBlock* true_bb = BasicBlock::Create(TheContext, "when true", parent_func);
	// BasicBlock* false_bb = BasicBlock::Create(TheContext, "when false", parent_func);
	// BasicBlock* merge_bb = BasicBlock::Create(TheContext, "after if", parent_func);
	// Builder.CreateCondBr(cond_val, true_bb, false_bb);

	// Builder.SetInsertPoint(true_bb);
	// Builder.CreateCall(putsFunc, right_str);
	// Builder.CreateBr(merge_bb);

	// Builder.SetInsertPoint(false_bb);
	// Builder.CreateCall(putsFunc, wrong_str);
	// Builder.CreateBr(merge_bb);

	// Builder.SetInsertPoint(merge_bb);

	

	/* Invoke it */
	// Builder.CreateCall(putsFunc, helloWorldStr);

	/* Invoke it */
	std::vector<Value*> argv;
	argv.push_back(formatStr);
	argv.push_back(Builder.CreateLoad(Builder.CreateInBoundsGEP(Builder.CreateLoad(arraypointer), ref0)));
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