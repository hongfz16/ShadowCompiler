//
// Created by shadowiterator on 18-12-29.
//

#ifndef SRC_GLOBAL_H
#define SRC_GLOBAL_H

#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <json/json.h>

#include <stack>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include "grammar.hpp"

#define pub(a) push_back(a)
#define pob(a) pop_back(a)
#define mp(a, b) make_pair((a), (b))


class scVariable;
class scType;
class scBlock;

using std::string;
using std::vector;
using std::cout;
using std::endl;
using llvm::BasicBock;
using llvm::Value;
using llvm::Function;
using llvm::ArrayType;

typedef std::map<string, scVariable*> msv;
typedef std::map<string, scVariable*>::iterator msvi;
typedef std::map<string, scFunction*> msf;
typedef std::map<string, scFunction*>::iterator msfi;
typedef std::vector<scType*> vt;
typedef std::vector<scType*>::iterator vti;


#endif //SRC_GLOBAL_H
