//
// Created by shadowiterator on 18-12-29.
//

#include "typesystem.h"

scType::scType(Type* ttype):type(ttype) {

}

void scType::setName(string &tname) {
    name = tname;
}

TypeSystem::TypeSystem(LLVMContext &tllvmContext): llvmContext(tllvmContext) {
    types.clear();
    castTable.clear();
}

scType* TypeSystem::getType(Type *type) {
    for(vti it = types.begin(); it != types.end(); ++it)
        if((*it)->type == type)
            return (*it);
    types.pub(new scType(type));
}

void TypeSystem::setTypeName(scType *sctype, string &name) {
    sctype->setName(name);
}

void TypeSystem::addCast(scType * src, scType *tar, CastInst::CastOps cast) {
    castTable[mp(src, tar)] = cast;
}

string& TypeSystem::type2str(scType* type) {
    return type->name;
}

CastInst::CastOps* TypeSystem::getCast(scType *src, scType *tar) {
    auto it = castTable.find(mp(src, tar));
    if(it == castTable.end()) {
        return nullptr;
    } else {
        CastInst::CastOps castop = it->second;
        return &castop;
    }
    // return it == castTable.end()? nullptr: &(*it);
}
