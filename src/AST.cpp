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

using std::string;
using llvm::Type;
using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;
using llvm::Value;

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

    isarray = false;
    arraysize = 1;
//    bool isptr, isarray;
    for(++it; it != lst.rend(); ++it) {
        shared_ptr<scNDeclarationBody> ptr = *it;
        isarray = ptr->is_array;
        arraysize = ptr->size;
        if(ptr->is_ptr)
            llvm_type = llvm_type->getPointerTo();
        else if(ptr->is_array)
            llvm_type = ArrayType::get(llvm_type, ptr->size);
    }
    scType* sctype = context.typeSystem.getscType(llvm_type);
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
        allocaInst = context.builder.CreateAlloca(type, arraysize);
    }
    else {
        allocaInst = context.builder.CreateAlloca(type);
    }
    Value* rtnvalue = context.setIdentifier(varName, allocaInst, sctype);
    if(rtnvalue == nullptr) {
        logerr("Duplicated variable name!");
        exit(1);
    }
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

    assert(this->expressions!=nullptr);
    scFunction* sccallee = context.seekFunction(this->f_name);
    if(sccallee == nullptr) {
        this->logerr("Callee is empty!");
        exit(1);
    }
    Function* callee = sccallee->function;
    this->type = cscallee->retType;
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

    llvm::Function* func = (llvm::Function*)this->func_declaration->code_generate(context);
    assert(func != nullptr);
    llvm::BasicBlock* basicBlock = llvm::BasicBlock::Create(context.llvmContext, "entry", func, nullptr);
    context.builder.SetInsertPoint(basicBlock);

    context.pushBlock(basicBlock);
    this->block->code_generate(context);
    context.popBlock();
    
    return func;
}

llvm::Value* scNString::code_generate(scContext& context) {
    cout<<"generating " << class_name << endl;
    this->is_assignable = false;
    this->type = context.typeSystem.getscType(context.builder.getInt8PtrTy());
    return context.builder.CreateGlobalStringPtr(this->value.substr(1,this->value.size()-2));
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
    this->type = context.typeSystem.getscType(context.builder.getInt32Ty());
    return llvm::ConstantInt::get(context.llvmContext, llvm::APInt(32, value));
}

llvm::Value* scNDouble64Number::code_generate(scContext& context) {
    cout<<"generating "<<class_name<<endl;

    this->is_assignable = false;
    this->type = context.typeSystem.getscType(context.builder.getDoubleTy());
    return llvm::ConstantFP::get(context.builder.getDoubleTy(), this->value);
}

llvm::Value* scNChar::code_generate(scContext& context) {
    cout<<"generating "<<class_name<<endl;

    this->is_assignable = false;
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
    return scvar->value;
}
