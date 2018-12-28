#include "AST.hpp"
#include "context.h"
#include <iostream>

using namespace std;

extern int yyparse();
extern scNStatements* TopBlock;


int main() {
	yyparse();
	cout<<"Finish parsing"<<endl;
	TopBlock->print_debug(0);
	return 0;
}