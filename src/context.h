//
// Created by shadowiterator on 18-12-29.
//

#ifndef SRC_CONTEXT_H
#define SRC_CONTEXT_H

#include "global.h"
#include "typesystem.h"

//class scVariable;
//class scType;
//class scBlock;
//
//using std::string;
//using std::vector;
//using std::cout;
//using std::endl;
//using llvm::BasicBock;
//using llvm::Value;
//using llvm::Function;
//
//
//typedef std::map<string, scVariable*> msv;
//typedef std::map<string, scVariable*>::iterator msvi;
//typedef std::map<string, scFunction*> msf;
//typedef std::map<string, scFunction*>::iterator msfi;
//typedef std::vector<scType*> vt;
//typedef std::vector<scType*>::iterator vti;

class scVariable {
public:
    Value* value;
    scType* type;
public:
    scVariable(Value*, scType*);
};

class scFunction {
public:
    Function* function;
    scType* retType;
    vector<scType*> argTypes;
    scFunction(Function*, scType*, vt&);
};

class scBlock {
public:
    BasicBlock* block;
    scVariable* returnValue;
    msv VSymboltable;
    msf FSymboltable;

public:
    scBlock(BasicBlock*);
    ~scBlock();
//    void setReturnValue();
    Value* seekIdentifier(string&);
    Function* seekFunction(string&);
    Value* setIdentifier(string&, Value*, scType*);
    Function* setFunction(string&, Function*, scType*, vt&);

};

class scContext {
public:
    vector<scBlock*> blocks;
    LLVMContext llvmContext;
    IRBuilder<> builder;
    unique_ptr<Module> llvmModule;
    TypeSystem typeSystem;
public:
    scContext();
    ~scContext();
    scBlock* getLastBlock();
    void pushBlock(BasicBlock*);
    void popBlock();
    Value* seekIdentifier(string&);
    Function* seekFunction(string&);
    Value* setIdentifier(string&, Value*, scType*);
    Function* setFunction(string&, Function*, scType*, vt&);
    scBlock* getCurrentBlock();
    void setCurrentReturnValue(Value*, scType*);
    Type* number2sctype(int number);
};

#endif //SRC_CONTEXT_H
