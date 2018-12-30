//
// Created by shadowiterator on 18-12-29.
//

#include "context.h"
// #include "grammar.hpp"

scVariable::scVariable(Value* tvalue, scType* ttype): value(tvalue), type(ttype) {

}

scFunction::scFunction(Function *tfunction, scType* ttype, vt& tvt)
        : function(tfunction), retType(ttype), argTypes(tvt) {

}

scBlock::scBlock(BasicBlock* tblock): block(tblock), returnValue(nullptr)
{
    returnValue = nullptr;
    parent_function = nullptr;
    VSymboltable.clear();
    FSymboltable.clear();
}

scBlock::~scBlock()
{
    for(auto it = VSymboltable.begin(); it != VSymboltable.end(); ++it)
        delete (*it).second;
    for(auto it = FSymboltable.begin(); it != FSymboltable.end(); ++it)
        delete (*it).second;
}

scVariable* scBlock::seekIdentifier(string& name)
{
    msvi it = VSymboltable.find(name);
    return it == VSymboltable.end()? nullptr: it->second;
}

scFunction* scBlock::seekFunction(string& name)
{
    msfi it = FSymboltable.find(name);
    return it == FSymboltable.end()? nullptr: it->second;
}

scVariable* scBlock::setIdentifier(string &name, Value *value, scType *sctype) {
    if(VSymboltable.find(name) != VSymboltable.end())
        return nullptr;
    scVariable* p = new scVariable(value, sctype);
    VSymboltable[name] = p;
    return p;
}

scFunction* scBlock::setFunction(string &name, Function *function, scType* ttype, vt& tvt) {
    if(FSymboltable.find(name) != FSymboltable.end())
        return nullptr;
    scFunction* p = new scFunction(function, ttype, tvt);
    FSymboltable[name] = p;
    return p;
}

scContext::scContext(): builder(llvmContext), typeSystem(llvmContext) {
    llvmModule = unique_ptr<Module> (new Module("main", llvmContext));
}

scContext::~scContext() {
    for(auto it = blocks.begin(); it!=blocks.end(); ++it) {
        delete (*it);
    }

}

void scContext::pushBlock(BasicBlock *block) {
    blocks.pub(new scBlock(block));
}

void scContext::popBlock() {
    scBlock* scblock = blocks.back();
    delete scblock;
    blocks.pob();
}

void scContext::setCurrentReturnValue(Value* value, scType *type) {
    scVariable* rtn = (blocks.back())->returnValue;
    if(rtn != nullptr) {
        delete rtn;
    }
    (blocks.back())->returnValue = new scVariable(value, type);
}

scBlock* scContext::getCurrentBlock() {
    return blocks.size()? blocks.back(): nullptr;
}

scFunction* scContext::setFunction(string &name, Function *function, scType *rtntype, vt &tvt) {
    return blocks.back()->setFunction(name, function, rtntype, tvt);
}

scVariable* scContext::setIdentifier(string &name, Value *value, scType *type) {
    return blocks.back()->setIdentifier(name, value, type);
}

scVariable* scContext::seekIdentifier(string &name) {
    scVariable* rtn;
    for(auto it = blocks.rbegin(); it != blocks.rend(); ++it) {
        if((rtn = (*it)->seekIdentifier(name)) != nullptr)
            return rtn;
    }
    return nullptr;
}

scFunction* scContext::seekFunction(string &name) {
    scFunction* rtn;
    for(auto it = blocks.rbegin(); it != blocks.rend(); ++it) {
        if((rtn = (*it)->seekFunction(name)) != nullptr)
            return rtn;
    }
    return nullptr;
}

void scBlock::setParentFunction(Function *func) {
    parent_function = func;
}

Function* scBlock::getParentFunction() {
    return parent_function;
}