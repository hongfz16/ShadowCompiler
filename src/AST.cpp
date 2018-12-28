//
// Created by shadowiterator on 18-12-29.
//

#include "global.h"
#include "context.h"

Value* scNVariableDeclaration::codeGen(scContext& context) {
    // get info
    vector<scNDeclarationBody*> lst;

    for(scNDeclarationBody* ptr = dec_body; ptr; ptr = ptr->children)
        lst.pub(ptr);
    auto it = lst.rbegin();
    Type* type = context.number2type(this->type);
    string varName = (*it)->name;
    for(++it; it != lst.rend(); ++it) {
        scNDeclarationBody* ptr = *it;
        if(ptr->is_ptr)
            type = type->getPointerTo();
        else if(ptr->is_array)
            type = ArrayType::get(type, ptr->size);
    }
    scType* sctype = context.typeSystem.getType(type);

    //TODO: code gen
}