%{
	#include "AST.hpp"
	#include <stdio.h>
	NExprs* block;
	extern int yylex();
	void yyerror(const char* s)
	{
		printf("Error: %s\n", s);
	}
%}
%union
{
	NExprs* exprs;
	NExpr* expr;
	Symbol* symbol;
	std::string* string;
	int token;
}

%token <string> TIDENTIFIER TINTEGER TDOUBLE
%token <token> TEQUAL
%token <token> TLPAREN TRPAREN
%token <token> TPLUS TMINUS TMUL TDIV TAND TOR TXOR TMOD TNEG TSHIFTL TSHIFTR

%type <expr> number expr assign
%type <exprs> exprs program
%type <symbol> symbol

%left TPLUS TMINUS
%left TMUL TDIV TMOD

%start program

%%
program : exprs { block = $1; }
	;
exprs : expr { $$ = new NExprs(); $$->exprs.push_back($1); }
	| exprs expr { $1->exprs.push_back($2); }
	;
expr : assign
	| TLPAREN expr TRPAREN { $$ = $2; }
	| expr TPLUS expr { $$ = new Bop($1, $2, $3);}
	| expr TMINUS expr { $$ = new Bop($1, $2, $3); }
	| symbol { $$ = $1; }
	| number
	;
number : TINTEGER { $$ = new Number(atof($1->c_str())); }
	| TDOUBLE { $$ = new Number(atof($1->c_str())); }
	;
symbol : TIDENTIFIER { $$ = new Symbol(*$1); }
	;
assign : symbol TEQUAL expr { $$ = new Assign($1, $3); }
	;
%%