//
// Created by shadowiterator on 18-12-29.
//

#include "global.h"
#include "AST.hpp"
#include "grammar.hpp"

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

#define ISTYPE(value, id) (value->getType()->getTypeID() == id)

using llvm::Type;
using llvm::ConstantInt;
using llvm::ConstantFP;
using llvm::Value;
using llvm::ArrayRef;

using std::string;
using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;

static Value* to_boolean(scContext& context, Value* rawVal)
{
    if( ISTYPE(rawVal, Type::IntegerTyID) ){
        rawVal = context.builder.CreateIntCast(rawVal, Type::getInt1Ty(context.llvmContext), true);
        return context.builder.CreateICmpNE(rawVal, ConstantInt::get(Type::getInt1Ty(context.llvmContext), 0, true));
    }
    else if( ISTYPE(rawVal, Type::DoubleTyID) )
    {
        return context.builder.CreateFCmpONE(rawVal, ConstantFP::get(context.llvmContext, llvm::APFloat(0.0)));
    }else
        {
        return rawVal;
    }
}

Type* scContext::number2type(int number) {
    switch(number) {
        case TYINT:
            return builder.getInt32Ty();
        case TYDOUBLE:
            return builder.getDoubleTy();
        case TYFLOAT:
            return builder.getFloatTy();
        case TYCHAR:
            return builder.getInt8Ty();
        case TYVOID:
            return builder.getVoidTy();
        default:
            return nullptr;
    }
}

void try_to_print(shared_ptr<scNNode> ptr, int depth)
{
    if(ptr == nullptr)
        return;
    ptr->print_debug(depth+1);
}

void scNNode::print_depth(int depth)
{
    for(int i=0; i<depth; i++)
        // cout<<"->";
        if(i & 1)
            cout<<"--";
        else 
            cout<<">>";
}

void scNNode::logerr(const string &err_info) {
    cout << err_info << endl;
}

void scNType::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    this->print_depth(depth+1);
    cout<<this->type<<" "<<this->count<<endl;
}

void scNExpression::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
}

void scNDeclarationBody::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    this->print_depth(depth+1);
    cout<<this->is_array<<" "
    <<this->is_ptr<<" "
    <<this->size<<" "
    <<this->p_size<<" "
    <<this->name<<endl;

    try_to_print((shared_ptr<scNNode>)this->children, depth);
}

void scNStatements::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;



    for(auto it = this->statement_list.begin(); it != this->statement_list.end(); it++)
    {
        try_to_print((shared_ptr<scNNode>)(*it), depth);
    }
}

void scNEmptyStatement::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;
}

void scNBlock::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    try_to_print((shared_ptr<scNNode>)statements, depth);

}

void scNIfStatement::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    try_to_print((shared_ptr<scNNode>)this->expression, depth);
    try_to_print((shared_ptr<scNNode>)this->statement, depth);
}

void scNIfElseStatement::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    try_to_print((shared_ptr<scNNode>)this->expression, depth);
    try_to_print((shared_ptr<scNNode>)this->if_statement, depth);
    try_to_print((shared_ptr<scNNode>)this->else_statement, depth);
}

void scNForStatement::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;

    try_to_print((shared_ptr<scNNode>)this->init_expression, depth);
    try_to_print((shared_ptr<scNNode>)this->cond_expression, depth);
    try_to_print((shared_ptr<scNNode>)this->update_expression, depth);
    try_to_print((shared_ptr<scNNode>)this->statement, depth);
}

void scNWhileStatement::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    try_to_print((shared_ptr<scNNode>)this->expression, depth);
    try_to_print((shared_ptr<scNNode>)this->statement, depth);
}

void scNFunctionDeclaration::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    this->print_depth(depth+1);
    cout<<this->type<<endl;
    this->print_depth(depth+1);
    // cout<<func_name<<endl;
    try_to_print((shared_ptr<scNNode>)this->dec_body, depth);
    try_to_print((shared_ptr<scNNode>)this->param_list, depth);
}

void scNFunctionDefinition::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    try_to_print((shared_ptr<scNNode>)this->func_declaration, depth);
    try_to_print((shared_ptr<scNNode>)this->block, depth);
}

void scNReturnStatement::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    try_to_print((shared_ptr<scNNode>)this->expression, depth);
}

void scNVariableDeclaration::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    try_to_print((shared_ptr<scNNode>)this->dec_body, depth);
}

void scNParams::print_debug(int depth)
{
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    for(auto it=this->param_list.begin(); it != this->param_list.end(); it++)
    {
        try_to_print((shared_ptr<scNNode>)(*it), depth);
    }
}

void scNIdentifier::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    this->print_depth(depth+1);
    cout<<this->name<<endl;
}

void scNFunctionCall::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    this->print_depth(depth+1);
    cout<<this->f_name<<endl;
    try_to_print((shared_ptr<scNNode>)this->expressions, depth);
}

void scNExpressions::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    for(auto it = expression_list.begin(); it!=expression_list.end(); ++it) {
        try_to_print((shared_ptr<scNNode>)(*it), depth);
    }
}

void scNArrayExpression::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    try_to_print((shared_ptr<scNNode>)target_expression, depth);
    try_to_print((shared_ptr<scNNode>)index_expression, depth);
}

void scNString::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    this->print_depth(depth+1);
    cout<<this->value<<endl;
}

void scNChar::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    this->print_depth(depth+1);
    cout<<this->value<<endl;
}

void scNInt32Number::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    this->print_depth(depth+1);
    cout<<this->value<<endl;
}

void scNDouble64Number::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    this->print_depth(depth+1);
    cout<<this->value<<endl;
}

void scNAssignment::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    try_to_print((shared_ptr<scNNode>)left_expression, depth);
    try_to_print((shared_ptr<scNNode>)right_expression, depth);
}

void scNBinaryExpression::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    try_to_print((shared_ptr<scNNode>)left_expression, depth);
    this->print_depth(depth+1);
    cout<<this->b_op<<endl;
    try_to_print((shared_ptr<scNNode>)right_expression, depth);
}

void scNUnaryExpression::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    this->print_depth(depth+1);
    cout<<this->u_op<<endl;
    try_to_print((shared_ptr<scNNode>)expression, depth);
}

void scNReferenceExpression::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    this->print_depth(depth+1);
    cout<<"&"<<endl;
    try_to_print((shared_ptr<scNNode>)expression, depth);
}

void scNDereferenceExpression::print_debug(int depth) {
    this->print_depth(depth);
    cout<<this->class_name<<endl;
    this->print_depth(depth+1);
    cout<<"*"<<endl;
    try_to_print((shared_ptr<scNNode>)expression, depth);
}

scType* getTypeFromDeclarationBody(shared_ptr<scNDeclarationBody> head_ptr, int type, scContext& context, bool& isarray, int& arraysize, string& name) {
    // get info
    vector<shared_ptr<scNDeclarationBody> > lst;

    for(shared_ptr<scNDeclarationBody> ptr = head_ptr; ptr; ptr = ptr->children)
        lst.pub(ptr);
    auto it = lst.rbegin();
    Type* llvm_type = context.number2type(type);
    name = (*it)->name;
    cout<<"size of lst: "<<name<<" "<<lst.size()<<endl;

    isarray = false;
    arraysize = 1;
//    bool isptr, isarray;
    for(++it; it != lst.rend(); ++it) {
        shared_ptr<scNDeclarationBody> ptr = *it;
        isarray = ptr->is_array;
        arraysize = ptr->size;
        if(ptr->is_ptr) {
            cout<<"getPointerTo"<<endl;
            llvm_type = llvm_type->getPointerTo();
        }
        else if(ptr->is_array) {
            cout<<"getArray"<<endl;
            llvm_type = ArrayType::get(llvm_type, ptr->size);
        }
    }
    cout<<"Final type: "<<llvm_type->getTypeID()<<endl;
    scType* sctype = context.typeSystem.getscType(llvm_type);
    cout<<"scType final: "<<sctype->type->getTypeID()<<endl;
    return sctype;
}

llvm::Value* scNVariableDeclaration::code_generate(scContext& context) {
    cout<<"generating " << class_name << endl;

    bool isarray = false;
    int arraysize = 1;
    string varName;
    scType* sctype = getTypeFromDeclarationBody(dec_body, this->type, context, isarray, arraysize, varName);
    Type* type = context.typeSystem.getllvmType(sctype);
    Value* allocaInst = nullptr;
    if(isarray) {
        allocaInst = context.builder.CreateAlloca(type);//, arraysize);
    }
    else {
        allocaInst = context.builder.CreateAlloca(type);
    }
    scVariable* rtnvalue = context.setIdentifier(varName, allocaInst, sctype);
    if(rtnvalue == nullptr) {
        logerr("Duplicated variable name!");
        exit(1);
    }
    cout<<"scType variable_declaration: "<<sctype->type->getTypeID()<<endl;
    return allocaInst;
}

llvm::Value* scNFunctionDeclaration::code_generate(scContext& context) {
    cout<<"generating " << class_name << endl;

    //get return type
    scFunction* seeked_func = context.seekFunction(dec_body->name);
    if(seeked_func!=nullptr) {
        return seeked_func->function;
    }
    bool isarray;
    int arraysize;
    string name;
    scType* return_sctype = getTypeFromDeclarationBody(dec_body, this->type, context, isarray, arraysize, name);
    //get param type
    std::vector<Type*> param_llvm_types;
    std::vector<scType*> param_sctypes;
    if(param_list!=nullptr) {
        for(auto it = param_list->param_list.begin(); it!=param_list->param_list.end(); ++it) {
            // if(this->is_definition) {
            //     cout<<"is_definition"<<endl;
            //     (*it)->code_generate(context);
            // }
            scType* param_sctype = getTypeFromDeclarationBody((*it)->dec_body, (*it)->type, context, isarray, arraysize, name);
            param_llvm_types.push_back(context.typeSystem.getllvmType(param_sctype));
            param_sctypes.push_back(param_sctype);
        }
    }
    llvm::ArrayRef<Type*> param_llvm_types_ref(param_llvm_types);
    llvm::FunctionType *func_type = llvm::FunctionType::get(context.builder.getInt32Ty(), param_llvm_types_ref, true);
    llvm::Function *func = llvm::Function::Create(func_type, llvm::GlobalValue::ExternalLinkage, dec_body->name.c_str(), context.llvmModule.get());
    context.setFunction(dec_body->name, func, return_sctype, param_sctypes);
    return func;
}

llvm::Value* scNFunctionCall::code_generate(scContext& context) {
    cout<<"generating " << class_name << endl;

    this->is_assignable = false;
    this->before_value = nullptr;

    assert(this->expressions!=nullptr);
    scFunction* sccallee = context.seekFunction(this->f_name);
    if(sccallee == nullptr) {
        this->logerr("Callee is empty!");
        exit(1);
    }
    Function* callee = sccallee->function;
    this->type = sccallee->retType;
    // if(callee->arg_size() != this->expressions->expression_list.size()) {
    //     this->logerr("Argument number does not match!");
    //     exit(1);
    // }
    std::vector<llvm::Value*> argsv;
    for(auto it = this->expressions->expression_list.begin(); it != this->expressions->expression_list.end(); ++it) {
        llvm::Value* value = (*it)->code_generate(context);
        if(value==nullptr) {
            this->logerr("Argument value can not be nullptr!");
            exit(1);
        }
        argsv.push_back(value);
    }
    string call = "call_";
    return context.builder.CreateCall(callee, argsv, call+this->f_name);
}

llvm::Value* scNStatements::code_generate(scContext& context) {
    cout<<"generating " << class_name << endl;

    llvm::Value* value;
    for(auto it = this->statement_list.begin(); it != this->statement_list.end(); ++it) {
        value = (*it)->code_generate(context);
        cout <<"statement done"<<endl;
    }
    return value;
}

llvm::Value* scNFunctionDefinition::code_generate(scContext& context) {
    cout<<"generating " << class_name << endl;

    // this->func_declaration->is_definition = true;
    llvm::Function* func = (llvm::Function*)this->func_declaration->code_generate(context);
    assert(func != nullptr);
    
    llvm::BasicBlock* basicBlock = llvm::BasicBlock::Create(context.llvmContext, "entry", func, nullptr);
    context.builder.SetInsertPoint(basicBlock);

    context.pushBlock(basicBlock);
    // scFunction* scfunc = context.seekFunction(this->func_declaration->dec_body->name);
    // vector<scType*>& argssctype = scfunc->argTypes;
    
    auto origin_arg = this->func_declaration->param_list->param_list.begin();
    for(auto &ir_arg_it: func->args()){
        ir_arg_it.setName((*origin_arg)->dec_body->name);
        (*origin_arg)->code_generate(context);
        scVariable* scvar = context.seekIdentifier((*origin_arg)->dec_body->name);
        context.builder.CreateStore(&ir_arg_it, scvar->value, false);
        ++origin_arg;
    }
    this->block->code_generate(context);

    context.popBlock();
    
    return func;
}

llvm::Value* scNString::code_generate(scContext& context) {
    cout<<"generating " << class_name << endl;
    this->is_assignable = false;
    this->before_value = nullptr;
    llvm::Type* llvm_type = context.builder.getInt8PtrTy();
    this->type = context.typeSystem.getscType(llvm_type);
    cout<<"string before build "<<this->value.substr(1,this->value.size()-2)<<endl;
    Value* return_value = context.builder.CreateGlobalStringPtr(this->value.substr(1,this->value.size()-2));
    cout<<"string return"<<endl;
    return return_value;
}

llvm::Value* scNBlock::code_generate(scContext &context) {
    cout<<"generating " << class_name << endl;

    context.pushBlock(context.getCurrentBlock()->block);
    assert(statements != nullptr);
    statements->code_generate(context);
    context.popBlock();
    return nullptr;
}

llvm::Value* scNReturnStatement::code_generate(scContext& context) {
    cout<<"generating "<<class_name<<endl;

    llvm::Value* value = this->expression->code_generate(context);
    return context.builder.CreateRet(value);
}

llvm::Value* scNInt32Number::code_generate(scContext &context) {
    cout<<"generating "<<class_name<<endl;

    this->is_assignable = false;
    this->before_value = nullptr;
    this->type = context.typeSystem.getscType(context.builder.getInt32Ty());
    return llvm::ConstantInt::get(context.llvmContext, llvm::APInt(32, value));
}

llvm::Value* scNDouble64Number::code_generate(scContext& context) {
    cout<<"generating "<<class_name<<endl;

    this->is_assignable = false;
    this->before_value = nullptr;
    this->type = context.typeSystem.getscType(context.builder.getDoubleTy());
    return llvm::ConstantFP::get(context.builder.getDoubleTy(), this->value);
}

llvm::Value* scNChar::code_generate(scContext& context) {
    cout<<"generating "<<class_name<<endl;

    this->is_assignable = false;
    this->before_value = nullptr;
    this->type = context.typeSystem.getscType(context.builder.getInt8Ty());
    int c = this->value[1];
    return llvm::ConstantInt::get(context.builder.getInt8Ty(), c);
}

llvm::Value* scNIdentifier::code_generate(scContext& context) {
    cout<<"generating "<<class_name<<endl;

    this->is_assignable = true;
    scVariable* scvar = context.seekIdentifier(this->name);
    assert(scvar!=nullptr);
    this->type = scvar->type;
    this->before_value = scvar->value;
    cout<<"seeked type: "<<this->type->type->getTypeID()<<endl;
    if(scvar->type->type->getTypeID() == Type::ArrayTyID) {
        cout<<this->name<<" is array"<<endl;
        return scvar->value;
    }
    // } else if(scvar->value->getType()->getTypeID() == Type::ArrayTyID) {
    //     cout<<"is array"<<endl;
    //     ArrayRef<Value*> ref = {}
    // }
    // cout<<"Identifier: "<<this->name<<" "<<scvar->value->getType()->getTypeID()<<endl;
    return context.builder.CreateLoad(scvar->value);
}

llvm::Value* scNAssignment::code_generate(scContext& context) {
    cout<<"generating "<<class_name<<endl;

    llvm::Value* src_llvm_value = right_expression->code_generate(context);
    llvm::Value* dst_llvm_value = left_expression->code_generate(context);

    if(!left_expression->is_assignable) {
        this->logerr("Cannot assign value to a right value.");
        exit(1);
    }

    this->is_assignable = true;
    this->type = left_expression->type;

    assert(right_expression->type != nullptr);
    assert(left_expression->type != nullptr);
    cout<<"left_expression type "<<left_expression->type->type->getTypeID()<<endl;
    cout<<"right_expression type "<<right_expression->type->type->getTypeID()<<endl;
    // if(right_expression->type != left_expression->type) {
        src_llvm_value = context.typeSystem.getCast(right_expression->type, 
            left_expression->type, src_llvm_value, context.getCurrentBlock()->block);
        // cout<<left_expression->type->type<<endl;
        assert(src_llvm_value!=nullptr);
        cout<<"casted"<<endl;
    // }

    this->before_value = left_expression->before_value;
    assert(this->before_value!=nullptr);

    context.builder.CreateStore(src_llvm_value, left_expression->before_value);
    return dst_llvm_value;
}

llvm::Value* scNArrayExpression::code_generate(scContext& context) {
    cout<<"generating "<<class_name<<endl;

    this->is_assignable = true;
    Value* indexValue = index_expression->code_generate(context);
    Value* targetValue = target_expression->code_generate(context);
    this->type = context.typeSystem.getscType(target_expression->type->type->getArrayElementType());

//    assert(target_expression) check is pointer
    // assert(targetValue->getType()->getTypeID() == Type::PointerTyID);
    llvm::ArrayRef<Value*> ref = {llvm::ConstantInt::get(Type::getInt64Ty(context.llvmContext), 0),indexValue};
    before_value = context.builder.CreateInBoundsGEP(targetValue, ref, "array-op");
    
    return context.builder.CreateLoad(before_value);
}

llvm::Value* scNReferenceExpression::code_generate(scContext& context) {
    cout<<"generating "<<class_name<<endl;
    
    this->is_assignable = false;
    before_value = nullptr;
    Value* value = expression->code_generate(context);
    this->type = context.typeSystem.getscType(expression->type->type->getPointerTo());
    assert(expression->before_value != nullptr);
    return expression->before_value;
}

llvm::Value* scNDereferenceExpression::code_generate(scContext& context) {
    cout<<"generating "<<class_name<<endl;
    
    this->is_assignable = true;
    before_value = expression->code_generate(context);
    this->type = context.typeSystem.getscType(expression->type->type->getPointerElementType());
    return context.builder.CreateLoad(before_value);
}

llvm::Value* scNContinueStatement::code_generate(scContext &context) {
    cout<<"generating "<<class_name<<endl;
    
    BasicBlock* block = context.getCurrentContinueToBlock();
    assert(block != nullptr);
    return context.builder.CreateBr(block);
}

llvm::Value* scNBreakStatement::code_generate(scContext &context) {
    cout<<"generating "<<class_name<<endl;
    
    BasicBlock* block = context.getCurrentBreakToBlock();
    assert(block != nullptr);
    return context.builder.CreateBr(block);
}

llvm::Value* scNIfStatement::code_generate(scContext& context){
    cout<<"generating "<<class_name<<endl;

    Value* condVal = this->expression->code_generate(context);
    if(!condVal)
    {
        return nullptr;
    }
    condVal = to_boolean(context, condVal);
    Function* parFunction = context.getCurrentBlock()->getParentFunction();

    BasicBlock* trueBB = BasicBlock::Create(context.llvmContext, "if_true", parFunction);
    BasicBlock* contBB = BasicBlock::Create(context.llvmContext, "if_cont");

    context.builder.CreateCondBr(condVal, trueBB, contBB);

    context.builder.SetInsertPoint(trueBB);
    context.pushBlock(trueBB);
    this->statement->code_generate(context);
    context.popBlock();

    trueBB = context.builder.GetInsertBlock();
    if(trueBB->getTerminator() == nullptr)
    {
        context.builder.CreateBr(contBB);
    }

    parFunction->getBasicBlockList().push_back(contBB);
    context.builder.SetInsertPoint(contBB);
    return nullptr;
}

llvm::Value* scNIfElseStatement::code_generate(scContext& context){
    cout<<"generating "<<class_name<<endl;

    Value* condVal = this->expression->code_generate(context);
    if(!condVal)
    {
        return nullptr;
    }
    condVal = to_boolean(context, condVal);
    Function* parFunction = context.getCurrentBlock()->getParentFunction();

    BasicBlock* trueBB = BasicBlock::Create(context.llvmContext, "if_true", parFunction);
    BasicBlock* falseBB = BasicBlock::Create(context.llvmContext, "if_false");
    BasicBlock* contBB = BasicBlock::Create(context.llvmContext, "if_cont");

    context.builder.CreateCondBr(condVal, trueBB, falseBB);

    context.builder.SetInsertPoint(trueBB);
    context.pushBlock(trueBB);
    this->if_statement->code_generate(context);
    context.popBlock();

    trueBB = context.builder.GetInsertBlock();
    if(trueBB->getTerminator() == nullptr)
    {
        context.builder.CreateBr(contBB);
    }

    parFunction->getBasicBlockList().push_back(falseBB);
    context.builder.SetInsertPoint(falseBB);
    context.pushBlock(falseBB);     // different
    this->else_statement->code_generate(context);
    context.popBlock();
    context.builder.CreateBr(contBB);

    parFunction->getBasicBlockList().push_back(contBB);
    context.builder.SetInsertPoint(contBB);
    return nullptr;
}

llvm::Value* scNForStatement::code_generate(scContext& context){
    cout<<"generating "<<class_name<<endl;
    
    Function* parFunction = context.getCurrentBlock()->getParentFunction();
    BasicBlock* condBB = BasicBlock::Create(context.llvmContext, "loop_condition");
    BasicBlock* updateBB = BasicBlock::Create(context.llvmContext, "loop_update");
    BasicBlock* loopBB = BasicBlock::Create(context.llvmContext, "loop_body");
    BasicBlock* contBB = BasicBlock::Create(context.llvmContext, "loop_cont");

    if(this->init_expression)
    {
        this->init_expression->code_generate(context);
    }

    parFunction->getBasicBlockList().push_back(condBB);
    context.builder.SetInsertPoint(condBB);
    context.pushBlock(condBB);
    Value* condVal = this->cond_expression->code_generate(context);
    context.popBlock();
    if(!condVal)
    {
        this->logerr("invalid condition");
        exit(1);
    }
    condVal = to_boolean(context, condVal);
    context.builder.CreateCondBr(condVal, loopBB, contBB);
//    context.popBlock();

    parFunction->getBasicBlockList().push_back(loopBB);
    context.builder.SetInsertPoint(loopBB);
    context.pushBlock(loopBB);
    context.breakToBlocks.push_back(contBB);
    context.continueToBlocks.push_back(updateBB);
    this->statement->code_generate(context);
    context.popBlock();

    parFunction->getBasicBlockList().push_back(updateBB);
    context.builder.SetInsertPoint(updateBB);
    context.pushBlock(updateBB);
    if(this->update_expression)
    {
        this->update_expression->code_generate(context);
    }

//    context.builder.CreateBr(condBB);

    condVal = this->cond_expression->code_generate(context);
    context.popBlock();
    condVal = to_boolean(context, condVal);
    context.builder.CreateCondBr(condVal, loopBB, contBB);

    parFunction->getBasicBlockList().push_back(contBB);
    context.builder.SetInsertPoint(contBB);
    context.breakToBlocks.pop_back();
    context.continueToBlocks.pop_back();
    return nullptr;

}

llvm::Value* scNWhileStatement::code_generate(scContext& context){
    cout<<"generating "<<class_name<<endl;

    Function* parFunction = context.getCurrentBlock()->getParentFunction();
    BasicBlock* condBB = BasicBlock::Create(context.llvmContext, "loop_condition");
    BasicBlock* loopBB = BasicBlock::Create(context.llvmContext, "loop_body");
    BasicBlock* contBB = BasicBlock::Create(context.llvmContext, "loop_cont");

    parFunction->getBasicBlockList().push_back(condBB);
    context.builder.SetInsertPoint(condBB);
    context.pushBlock(condBB);
    Value* condVal = this->expression->code_generate(context);
    context.popBlock();
    if(!condVal)
    {
        return nullptr;
    }
    condVal = to_boolean(context, condVal);
    context.builder.CreateCondBr(condVal, loopBB, contBB);

    context.builder.SetInsertPoint(loopBB);
    context.pushBlock(loopBB);
    context.breakToBlocks.push_back(contBB);
    context.continueToBlocks.push_back(condBB);
    this->statement->code_generate(context);
    context.popBlock();

    condVal = this->expression->code_generate(context);
    condVal = to_boolean(context, condVal);
    context.builder.CreateCondBr(condVal, loopBB, contBB);

    parFunction->getBasicBlockList().push_back(contBB);
    context.builder.SetInsertPoint(contBB);
    context.breakToBlocks.pop_back();
    context.continueToBlocks.pop_back();
    return nullptr;

}

llvm::Value* scNBinaryExpression::code_generate(scContext& context){
    cout<<"generating "<<class_name<<endl;

    this->is_assignable = false;
    this->before_value = nullptr;

    Value* lVal = this->left_expression->code_generate(context);
    Value* rVal = this->right_expression->code_generate(context);

    if(!lVal || !rVal)
    {
        this->logerr("lack operation num !");
        exit(1);
    }

    // pointer operation
    if(lVal->getType()->getTypeID() == Type::PointerTyID)
    {
        if(rVal->getType()->getTypeID() != Type::IntegerTyID)
        {
            this->logerr("wrong type for pointer operation");
            exit(1);
        }
        else
        {
            ArrayRef<Value*> opRef = {rVal};
            switch (this->b_op)
            {
                case TPLUS:
                case TMINUS:
                    this->type = this->left_expression->type;
                    return context.builder.CreateInBoundsGEP(lVal, opRef, "pointer add");
                default:
                    this->logerr("wrong type for pointer operation");
                    exit(1);
                    break;
            }
        }
    }
    if(rVal->getType()->getTypeID() == Type::PointerTyID)
    {
        if(lVal->getType()->getTypeID() != Type::IntegerTyID)
        {
            this->logerr("wrong type for pointer operation");
            exit(1);
        }
        else
        {
            ArrayRef<Value*> opRef = {lVal};
            switch (this->b_op)
            {
                case TPLUS:
                case TMINUS:
                    this->type = this->right_expression->type;
                    return context.builder.CreateInBoundsGEP(lVal, opRef, "pointer add");
                default:
                    this->logerr("wrong type for pointer operation");
                    exit(1);
            }
        }
    }

    bool floatOp = false;
    if(lVal->getType()->getTypeID() == Type::DoubleTyID)
    {
        floatOp = true;
//        this->type = this->left_expression->type;
        if(rVal->getType()->getTypeID() != Type::DoubleTyID)
        {
            rVal = context.builder.CreateUIToFP(rVal, Type::getDoubleTy(context.llvmContext));
        }
    }
    if(rVal->getType()->getTypeID() == Type::DoubleTyID)
    {
        floatOp = true;
//        this->type = this->right_expression->type;
        if(lVal->getType()->getTypeID() != Type::DoubleTyID)
        {
            lVal = context.builder.CreateUIToFP(lVal, Type::getDoubleTy(context.llvmContext));
        }
    }

    if(floatOp)
    {
        this->type = context.typeSystem.getscType(context.builder.getDoubleTy());
        switch (this->b_op)
        {
            case TCEQ:
                return context.builder.CreateFCmpOEQ(lVal, rVal, "equal");
            case TCNE:
                return context.builder.CreateFCmpONE(lVal, rVal, "unequal");
            case TCLT:
                return context.builder.CreateFCmpOLT(lVal, rVal, "smaller");
            case TCLE:
                return context.builder.CreateFCmpOLE(lVal, rVal, "smaller or eq");
            case TCGT:
                return context.builder.CreateFCmpOGT(lVal, rVal, "greater");
            case TCGE:
                return context.builder.CreateFCmpOGE(lVal, rVal, "greater or eql");
            case TPLUS:
                return context.builder.CreateFAdd(lVal, rVal, "add");
            case TMINUS:
                return context.builder.CreateFSub(lVal, rVal, "sub");
            case TMUL:
                return context.builder.CreateFMul(lVal, rVal, "mul");
            case TDIV:
                return context.builder.CreateFDiv(lVal, rVal, "div");
            case TANDAND:
                lVal = to_boolean(context, lVal);
                rVal = to_boolean(context, rVal);
                this->type = context.typeSystem.getscType(context.builder.getInt1Ty());
                return context.builder.CreateAnd(lVal, rVal, "div");
            case TOROR:
                lVal = to_boolean(context, lVal);
                rVal = to_boolean(context, rVal);
                this->type = context.typeSystem.getscType(context.builder.getInt1Ty());
                return context.builder.CreateOr(lVal, rVal, "div");
            default:
                this->logerr("unexpected operator type in double op");
                exit(1);
        }
    }
    else
    {
        this->type = context.typeSystem.getscType(context.builder.getInt32Ty());
        switch (this->b_op)
        {
            case TCEQ:
                return context.builder.CreateICmpEQ(lVal, rVal, "equal");
            case TCNE:
                return context.builder.CreateICmpNE(lVal, rVal, "unequal");
            case TCLT:
                return context.builder.CreateICmpSLT(lVal, rVal, "smaller");
            case TCLE:
                return context.builder.CreateICmpSLE(lVal, rVal, "smaller or eq");
            case TCGT:
                return context.builder.CreateICmpSGT(lVal, rVal, "greater");
            case TCGE:
                return context.builder.CreateICmpSGE(lVal, rVal, "greater or eql");
            case TPLUS:
                return context.builder.CreateAdd(lVal, rVal, "add");
            case TMINUS:
                return context.builder.CreateSub(lVal, rVal, "sub");
            case TMUL:
                return context.builder.CreateMul(lVal, rVal, "mul");
            case TDIV:
                return context.builder.CreateSDiv(lVal, rVal, "div");
            case TANDAND:
                lVal = to_boolean(context, lVal);
                rVal = to_boolean(context, rVal);
                return context.builder.CreateAnd(lVal, rVal, "div");
            case TOROR:
                lVal = to_boolean(context, lVal);
                rVal = to_boolean(context, rVal);
                return context.builder.CreateOr(lVal, rVal, "div");
            case TAND:
                return context.builder.CreateAnd(lVal, rVal, "and");
            case TOR:
                return context.builder.CreateOr(lVal, rVal, "or");
            case TXOR:
                return context.builder.CreateXor(lVal, rVal, "xor");
            case TMOD:
                return context.builder.CreateSRem(lVal, rVal, "mod");
            case TSHIFTL:
                return context.builder.CreateShl(lVal, rVal, "shift left");
            case TSHIFTR:
                return context.builder.CreateLShr(lVal, rVal, "shift right");
            default:
                this->logerr("unexpected operator type in int op");
                exit(1);
        }
    }
    this->logerr("unexpected operation error");
    exit(1);
}

llvm::Value* scNUnaryExpression::code_generate(scContext &context) {
    cout<<"generating "<<class_name<<endl;
   
    this->is_assignable = false;
    this->before_value = nullptr;
    Value* opVal = this->expression->code_generate(context);

    this->type = this->expression->type;

    if(opVal->getType()->getTypeID() == Type::PointerTyID)
    {
        this->logerr("unexpected operation num type");
        exit(1);
    }

    bool floatOp = false;
    if(opVal->getType()->getTypeID() == Type::DoubleTyID)
    {
        floatOp = true;
    }

    if(floatOp)
    {
        switch (this->u_op)
        {
            case TMINUS:
                return context.builder.CreateFNeg(opVal);
            case TEXCLAMATION:
                opVal = to_boolean(context, opVal);
                this->type = context.typeSystem.getscType(context.builder.getInt32Ty());
                return context.builder.CreateNot(opVal);
            default:
                this->logerr("unexpected operator type in double op");
                exit(1);
        }
    }
    else
    {
        switch (this->u_op)
        {
            case TMINUS:
                return context.builder.CreateNeg(opVal);
            case TEXCLAMATION:
                opVal = to_boolean(context, opVal);
                return context.builder.CreateNot(opVal);
            default:
                this->logerr("unexpected operator type in integer op");
                exit(1);
        }
    }

}
