//
// Created by shadowiterator on 18-12-29.
//

#ifndef SRC_TYPESYSTEM_H
#define SRC_TYPESYSTEM_H

#include "global.h"

using llvm::CastInst;

typedef pair<scType*, scType*> ptt;
typedef map<ptt, CastInst::CastOps> CastTable;


class TypeSystem {
public:
    LLVMContext& llvmContext;
    vt types;
    CastTable castTable;

    TypeSystem(LLVMContext&);

    scType* getscType(Type*);
    Type* getllvmType(scType*);
    void setTypeName(scType*, string&);
    void addCast(scType*, scType*, CastInst::CastOps);
    CastInst::CastOps* getCast(scType*, scType*);
    string& type2str(scType*);
};

class scType {
public:
    Type* type;
    string name;
    scType(Type*);
    void setName(string&);
//    scType(Type)
};

class TypeTable {


};

#endif //SRC_TYPESYSTEM_H
