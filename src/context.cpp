//
// Created by shadowiterator on 18-12-29.
//

#include "context.h"

scVariable::scVariable(Value* tvalue, scType* ttype): value(tvalue), type(ttype) {

}

scFunction::scFunction(Function *tfunction, scType* ttype, vt& tvt)
        : function(tfunction), retType(ttype), argTypes(tvt) {

}

scBlock::scBlock(BasicBlock* tblock): block(tblock), returnValue(nullptr)
{
    returnValue = nullptr;
    VSymboltable.clear();
    FSymboltable.clear();
}

scBlock::~scBlock()
{
    delete block;
}

Value* scBlock::seekIdentifier(string& name)
{
    msvi it = VSymboltable.find(name);
    return it == VSymboltable.end()? nullptr: it->second.value;
}

Function* scBlock::seekFunction(string& name)
{
    msfi = FSymboltable.find(name);
    return it == FSymboltable.end()? nullptr: it->second.function;
}

Value* scBlock::setIdentifier(string &name, Value *value, scType *sctype) {
    if(VSymboltable.find(name) != VSymboltable.end())
        return nullptr;
    VSymboltable[name] = new scVariable(value, sctype);
    return value;
}

Function* scBlock::setFunction(string &name, Function *function, scType* ttype, vt& tvt) {
    if(FSymboltable.find(name) != FSymboltable.end())
        return nullptr;
    FSymboltable[name] = new scFunction(function, ttype, tvt);
    return function;
}

scContext::scContext(): builder(llvmContext), TypeSystem(llvmContext) {
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
    rtnval = (blocks.back())->returnValue;
    if(rtn != nullptr) {
        delete rtn;
    }
    (blocks.back())->returnValue = new scVariable(value, type);
}

void scContext::getCurrentBlock() {
    return blocks.size()? blocks.back(): nullptr;
}

Function* scContext::setFunction(string &name, Function *function, scType *rtntype, vt &tvt) {
    return blocks.back()->setFunction(name, function, rtntype, tvt);
}

Value* scContext::setIdentifier(string &name, Value *value, Type *type) {
    return blocks.back()->setIdentifier(name, value, type);
}

Value* scContext::seekIdentifier(string &name) {
    Value* rtn;
    for(auto it = blocks.rbegin(); it != blocks.rend(); ++it) {
        if((rtn = (*it)->seekIdentifier(name)) != nullptr)
            return rtn;
    }
    return nullptr;
}

Function* scContext::seekFunction(string &name) {
    Function* rtn;
    for(auto it = blocks.rbegin(); it != blocks.rend(); ++it) {
        if((rtn = (*it)->seekFunction(name)) != nullptr)
            return rtn;
    }
    return rtn;
}
