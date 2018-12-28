%{
	#include ""
	#include <stdio.h>
	scStatements* TopBlock;
	extern int yylex();
	void yyerror(const char* s)
	{
		printf("Error: %s\n", s);
	}
%}
%union
{
	scStatements* statements;
	scStatement* statement;
	scDeclaration* declaration;
	scExpression* expression;
	scNumber* number;
	scDeclBody* decl_body;
	scType* type;
	scParamList* param_list;
	scExpressions* expressions;

	std::string* string;
	int token;
}

%token <string> TIDENTIFIER TINTEGER TDOUBLE TYINT TYDOUBLE TYFLOAT TYCHAR TYBOOL TYVOID TYSTRING TEXTERN TSTRING TCHAR
%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL TEXCLAMATION
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT TSEMICOLON TLBRACKET TRBRACKET TQUOTATION
%token <token> TPLUS TMINUS TMUL TDIV TAND TOR TXOR TMOD TNEG TNOT TSHIFTL TSHIFTR
%token <token> TIF TELSE TFOR TWHILE TRETURN TSTRUCT


%token <statements> statements program
%token <statement> statement block if_statement if_else_statement while_statement for_statement return_statement fdefinition empty_statement
%token <declaration> declaration variable_decl function_decl
%token <expression> expression unary_expression binary_expression reference_expression dereference_expression array_expression function_call assignment identifier
%token <number> number
%token <decl_body> decl_body
%token <type> super_type
%token <param_list> param_list
%token <expressions> expressions
%token <int> bop uop type


%left TPLUS TMINUS
%left TMUL TDIV TMOD

%start program

%%
program : statements { TopBlock = $1; }
		;

statements : statement { $$ = new scStatements();
						 $$->statement_list.push_back($1); }
		| statements statement { $1->statement_list.push_back($2); }
		;

statement : declaration TSEMICOLON { $$ = $1; }
		| expression TSEMICOLON { $$ = $1; }
		| block { $$ = $1; }
		| if_statement { $$ = $1; }
		| if_else_statement { $$ = $1; }
		| while_statement { $$ = $1; }
		| for_statement { $$ = $1; }
		| return_statement TSEMICOLON { $$ = $1; }
		| fdefinition { $$ = $1; }
		| empty_statement { $$ = $1; }
		;

expression : number { $$ = $1; }
		| unary_expression { $$ = $1; }
		| binary_expression { $$ = $1; }
		| reference_expression { $$ = $1; }
		| dereference_expression { $$ = $1; }
		| array_expression { $$ = $1; }
		| function_call { $$ = $1; }
		| TLPAREN expression TRPAREN { $$ = $2; }
		| identifier { $$ = $1; }
		| assignment { $$ = $1; }
		| TSTRING { $$ = new scString(*($1)); }
		| TCHAR { $$ = new scChar(*($1)); }
		;

declaration : variable_decl { $$ = $1; }
		| function_decl { $$ = $1; }
		;

variable_decl : type decl_body { $$ = new scVariableDeclaration($1, $2);}
		;

type    : TYINT { $$ = $1; }
		| TYDOUBLE { $$ = $1; }
		| TYFLOAT { $$ = $1; }
		| TYCHAR { $$ = $1; }
		| TYVOID { $$ = $1; }
		;

decl_body : TLPAREN decl_body TRPAREN { $$ = $2; }
		| TMUL decl_body { $$ = new scDeclarationBody();
						   $$->name = $1->name;
						   $$->isarray = false;
						   $$->isptr = true;
						   $$->size = 1;
						   $$->psize = $2->size;
						   $$->children = $2; }
		| decl_body TLBRACKET number TRPRACKET { $$ = new scDeclarationBody();
												 $$->name = $1->name;
												 $$->isarray = true;
												 $$->isptr = false;
												 $$->size = $1->size * int(number->value);
												 $$->children = $1; }
		| identifier { $$ = new scDeclarationBody();
					   $$->name = $1->name;
					   $$->isarry = false;
					   $$->isptr = false;
					   $$->size = 1;
					   $$->children = nullptr; }
		;

function_decl : super_type TIDENTIFIER TLPAREN param_list TRPAREN { $$ = new scFunctionDeclaration($1, *($2), $4); }
		| TEXTERN super_type TIDENTIFIER TLPAREN param_list TRPAREN { $$ = new scFunctionDeclaration($1, *($2), $4); }
		;

super_type : type { $$ = new scType($1); }
		| super_type TMUL { $1->type = type->getPointerTo();
							$$ = $1; }
		;

param_list : variable_decl { $$ = new scParams(); 
							 $$->param_list.push_back($1); }
		| param_list TCOMMA variable_decl { $1->param_list.push_back($3); 
											$$ = $1; }
		;

block : TLBRACE TRBRACE { $$ = new scBlock(); }
		| TLBRACE statements TLBRACE { $$->statements = $2; }
		;

if_statement : TIF TLPAREN expression TRPAREN statement { $$ = new scIfStatement($3, $5); }
		;

if_else_statement : TIF TLPAREN expression TRPAREN statement TELSE statement { $$ = new scIfElseStatement($3, $5, $7); }
		;

while_statement : TWHILE TLPAREN expression TRPAREN statement { $$ = new scWhileStatement($3, $5); }
		;

for_statement : TFOR TLPAREN expression TSEMICOLON expression TSEMICOLON expression TRPAREN statement { $$ = new scForStatement($3, $5, $7, $9); }
		;

return_statement : TRETURN expression { $$ = new scReturnStatement($2); }
		;

fdefinition : function_decl block { $$ = new scFunctionDefinition($1, $2); }
		;

empty_statement : TSEMICOLON { $$ = new scEmptyStatement(); }
		;

number : TINTEGER { $$ = new scInt32Number(atol($1->c_str())); }
		| TDOUBLE { $$ = new scDouble64Number(atof($1->c_str())); }
		;

assignment : expression TEQUAL expression { $$ = new scAssignment($1, $3); }
		;

identifier : TIDENTIFIER { $$ = new scIdentifier(*($1)); }
		;

function_call : identifier TLPAREN expressions TRPAREN { $$ = new scFunctionCall(*($1), $3); }
		;

expressions : expression { $$ = new scExpressions();
						   $$->expression_list.push_back($1); }
		| expressions TCOMMA expression { $1->expression_list.push_back($3);
										  $$ = $1; }
		;

binary_expression : expression bop expression { $$ = new scBinaryExpression($1, $3, $2); }
		;

bop 	: TCEQ { $$ = $1; }
		| TCNE { $$ = $1; }
		| TCLT { $$ = $1; }
		| TCLE { $$ = $1; }
		| TCGT { $$ = $1; }
		| TCGE { $$ = $1; }
		| TPLUS { $$ = $1; }
		| TMINUS { $$ = $1; }
		| TMUL { $$ = $1; }
		| TDIV { $$ = $1; }
		| TAND { $$ = $1; }
		| TOR { $$ = $1; }
		| TXOR { $$ = $1; }
		| TMOD { $$ = $1; }
		| TSHIFTR { $$ = $1; }
		| TSHIFTL { $$ = $1; }
		;

unary_expression : uop expression { $$ = new scUnaryExpression($2, $1); }
		;

uop 	: TMINUS { $$ = $1; }
		| TEXCLAMATION { $$ = $1; }
		| TWAVY { $$ = $1; }
		;

reference_expression : TAND expression { $$ = new scReferenceExpression($2); }
		;

dereference_expression : TMUL expression { $$ = new scDereferenceExpression($2); }
		;

array_expression : expression TLBRACKET expression TRBRACKET { $$ = new scArrayExpression($1, $3); }
		;

%%