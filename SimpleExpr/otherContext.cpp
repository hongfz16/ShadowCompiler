//
// Created by cs on 2017/5/28.
//

#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/Support/raw_ostream.h>
//#include <llvm/IR/Verifier.h>

#include "TypeSystem.h"
#include "otherContext.h"
#include "AST.hpp"

using legacy::PassManager;
#define ISTYPE(value, id) (value->getType()->getTypeID() == id)

/*
 * TODO:
 *       1. unary ops
 *       2. variable declaration list
 *
 *
 *
 */

//
static Type* TypeOf(const Symbol & type, CodeGenContext& context){        // get llvm::type of variable base on its identifier
    return context.typeSystem.getVarType(type);
}

// static Value* CastToBoolean(CodeGenContext& context, Value* condValue){

//     if( ISTYPE(condValue, Type::IntegerTyID) ){
//         condValue = context.builder.CreateIntCast(condValue, Type::getInt1Ty(context.llvmContext), true);
//         return context.builder.CreateICmpNE(condValue, ConstantInt::get(Type::getInt1Ty(context.llvmContext), 0, true));
//     }else if( ISTYPE(condValue, Type::DoubleTyID) ){
//         return context.builder.CreateFCmpONE(condValue, ConstantFP::get(context.llvmContext, APFloat(0.0)));
//     }else{
//         return condValue;
//     }
// }

// static llvm::Value* calcArrayIndex(shared_ptr<NArrayIndex> index, CodeGenContext &context){
//     auto sizeVec = context.getArraySize(index->arrayName->name);
//     cout << "sizeVec:" << sizeVec.size() << ", expressions: " << index->expressions->size() << endl;
//     assert(sizeVec.size() > 0 && sizeVec.size() == index->expressions->size());
//     auto expression = *(index->expressions->rbegin());

//     for(unsigned int i=sizeVec.size()-1; i>=1; i--){
//         auto temp = make_shared<NBinaryOperator>(make_shared<NInteger>(sizeVec[i]), TMUL, index->expressions->at(i-1));
// //        cout << "2" << endl;
//         expression = make_shared<NBinaryOperator>(temp, TPLUS, expression);
// //        cout << "3" << endl;
//     }

//     return expression->codeGen(context);
// }

void CodeGenContext::generateCode(NExprs& root) {
    cout << "Generating IR code" << endl;

    std::vector<Type*> sysArgs;
    FunctionType* mainFuncType = FunctionType::get(Type::getVoidTy(this->llvmContext), makeArrayRef(sysArgs), false);
    Function* mainFunc = Function::Create(mainFuncType, GlobalValue::ExternalLinkage, "main");
    BasicBlock* block = BasicBlock::Create(this->llvmContext, "entry");

    pushBlock(block);
    Value* retValue = root.codeGen(*this);
    popBlock();

    cout << "Code generate success" << endl;

    PassManager passManager;
    passManager.add(createPrintModulePass(outs()));
    passManager.run(*(this->theModule.get()));
    return;
}

std::unique_ptr<NExprs> LogError(const char *str) {
    fprintf(stderr, "LogError: %s\n", str);
    return nullptr;
}

Value *LogErrorV(string str){
    return LogErrorV(str.c_str());
}

Value *LogErrorV(const char *str) {
    LogError(str);
    return nullptr;
}
