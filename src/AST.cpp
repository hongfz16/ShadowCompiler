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
    try_to_print((shared_ptr<scNNode>)this->type, depth);
    this->print_depth(depth+1);
    cout<<func_name<<endl;
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

scType* getTypeFromDeclarationBody(shared_ptr<scNDeclarationBody> head_ptr, bool& isarray, int& arraysize, string& name)
{
    // get info
    vector<shared_ptr<scNDeclarationBody> > lst;

    for(shared_ptr<scNDeclarationBody> ptr = dec_body; ptr; ptr = ptr->children)
        lst.pub(ptr);
    auto it = lst.rbegin();
    Type* type = context.number2type(this->type);
    name = (*it)->name;

    isarray = false;
    arraysize = 1;
//    bool isptr, isarray;
    for(++it; it != lst.rend(); ++it) {
        shared_ptr<scNDeclarationBody> ptr = *it;
        isarray = ptr->is_array();
        arraysize = ptr->size;
        if(ptr->is_ptr)
            type = type->getPointerTo();
        else if(ptr->is_array)
            type = ArrayType::get(type, ptr->size);
    }
    scType* sctype = context.typeSystem.getType(type);
    return sctype;
}

Value* scNVariableDeclaration::code_generate(scContext& context) {
    bool isarray = false;
    int arraysize = 1;
    name varName;
    scType* sctype = getTypeFromDeclarationBody(dec_body, isarray, arraysize, varName);
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
        logerr("duplicated variable name!");
        exit(1);
    }
    return allocaInst;
}

void scNNode::logerr(const string &err_info) {
    cout << err_info << endl;
}