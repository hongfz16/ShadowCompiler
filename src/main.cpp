#include "AST.hpp"
#include "context.h"
#include <iostream>

using namespace std;

extern int yyparse();
extern scNStatements* TopBlock;

void writeModuleToFile(Module *module, string filename)
{
	std::ofstream std_file_stream(filename);
	raw_os_ostream file_stream(std_file_stream);
	module->print(file_stream, nullptr);
}

int main() {
	yyparse();
	cout<<"Finish parsing"<<endl;
	TopBlock->print_debug(0);
	scContext context;
	TopBlock->code_generate(context);
	string filename = "out.ll";
	writeModuleToFile(context.llvmModule, filename);
	return 0;
}