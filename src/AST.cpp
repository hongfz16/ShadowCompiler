//
// Created by shadowiterator on 18-12-29.
//

#include "global.h"
#include "AST.hpp"

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
    //TODO: code gen
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
    Function* seeked_func = context.seekFunction(dec_body->name);
    if(seeked_func!=nullptr) {
        return seeked_func;
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
    llvm::FunctionType *func_type = llvm::FunctionType::get(context.builder.getInt32Ty(), param_llvm_types_ref, false);
    llvm::Function *func = llvm::Function::Create(func_type, llvm::GlobalValue::ExternalLinkage, dec_body->name.c_str(), context.llvmModule.get());
    context.setFunction(dec_body->name, func, return_sctype, param_sctypes);
    return func;
}

llvm::Value* scNFunctionCall::code_generate(scContext& context) {
    cout<<"generating " << class_name << endl;

    assert(this->expressions!=nullptr);
    Function* callee = context.seekFunction(this->f_name);
    if(callee == nullptr) {
        this->logerr("Callee is empty!");
        exit(1);
    }
    if(callee->arg_size() != this->expressions->expression_list.size()) {
        this->logerr("Argument number does not match!");
        exit(1);
    }
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
    this->block->parent_function = func;
    llvm::BasicBlock* basicBlock = llvm::BasicBlock::Create(context.llvmContext, "entry", func, nullptr);
    context.builder.SetInsertPoint(basicBlock);

    if(this->block->statements!=nullptr) {
        this->block->statements->code_generate(context);
    }
    
    context.builder.CreateRet(llvm::ConstantInt::get(context.llvmContext, llvm::APInt(32, 0)));
    // this->block->code_generate(context);
    return func;
}

llvm::Value* scNString::code_generate(scContext& context) {
    cout<<"generating " << class_name << endl;
    return context.builder.CreateGlobalStringPtr(this->value);
}

llvm::Value* scNBlock::code_generate(scContext &context) {
    cout<<"generating " << class_name << endl;
    Function* par_func;

    if(this->parent_function != nullptr)
        par_func = this->parent_function;
    else
        par_func = context.getCurrentBlock()->getParentFunction();

    // BasicBlock* basicBlock = BasicBlock::Create(context.llvmContext, "entry", par_func, nullptr);
    // context.builder.SetInsertPoint(basicBlock);


    // context.pushBlock(basicBlock);
    // context.getCurrentBlock()->setParentFunction(par_func);
    // assert(statements != nullptr);
    if(statements!=nullptr) {
        statements->code_generate(context);
    }

    // cout<<"block gen done" << endl;
    // context.builder.CreateRet(llvm::ConstantInt::get(context.llvmContext, llvm::APInt(32, 1)));

    // context.popBlock();
    // context.builder.SetInsertPoint(context.getCurrentBlock()->block);
    // return basicBlock;
    return nullptr;
}
