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
    addCast(getscType(Type::getInt32Ty(tllvmContext)), getscType(Type::getFloatTy(tllvmContext)),llvm::CastInst::SIToFP);
    addCast(getscType(Type::getInt1Ty(tllvmContext)), getscType(Type::getInt32Ty(tllvmContext)),llvm::CastInst::SExt);
    addCast(getscType(Type::getInt32Ty(tllvmContext)), getscType(Type::getInt32Ty(tllvmContext)),llvm::CastInst::SExt);
    addCast(getscType(Type::getInt32Ty(tllvmContext)), getscType(Type::getInt8Ty(tllvmContext)),llvm::CastInst::Trunc);
    addCast(getscType(Type::getInt8Ty(tllvmContext)), getscType(Type::getInt32Ty(tllvmContext)),llvm::CastInst::SExt);
}

scType* TypeSystem::getscType(Type *type) {
    for(vti it = types.begin(); it != types.end(); ++it)
        if((*it)->type == type)
            return (*it);
    types.pub(new scType(type));
    return types.back();
}

Type* TypeSystem::getllvmType(scType * sctype) {
    return sctype == nullptr ? nullptr : sctype->type;
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

llvm::Value* TypeSystem::getCast(scType *src, scType *tar, Value* value, llvm::BasicBlock* block) {
    auto it = castTable.find(mp(src, tar));
    if(it == castTable.end()) {
        return nullptr;
    } else {
        CastInst::CastOps castop = it->second;
//        return &castop;
        return llvm::CastInst::Create(it->second, value, tar->type, "cast", block);
    }
    // return it == castTable.end()? nullptr: &(*it);
}
