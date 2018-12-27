#include "AST.hpp"
//#include "context.hpp"
#include "otherContext.h"
#include <iostream>

using namespace std;

extern int yyparse();
extern NExprs* block;


int main() {
	yyparse();
	cout<<"Finish parsing"<<endl;
	CodeGenContext context;
	// block->codeGen(context);
	context.generateCode(*block);
	return 0;
}