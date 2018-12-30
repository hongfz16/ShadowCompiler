%{
	#include "AST.hpp"
	#include <stdio.h>
	#include <string>
	scNStatements* TopBlock;
	extern int yylex();
	void yyerror(const char* s)
	{
		printf("Error: %s\n", s);
	}

	string toRawString(string& str) {
	    string rtn;
	    char tt;
	    for(int i = 0; i != str.size(); ++i) {
	        if(str[i] != '\\')
	            tt = str[i];
	        else {
	            ++i;
	            switch(str[i]) {
	                case 'a':
	                    tt = 7;
	                    break;
	                case 'b':
	                    tt = 8;
	                    break;
	                case 'n':
	                    tt = 10;
	                    break;
	                case '0':
	                    tt = 0;
	                    break;
	                case 't':
	                    tt = 9;
	                    break;
	                case '\\':
	                    tt = 92;
	                    break;
	                case '\'':
	                    tt = 39;
	                    break;
                    case '\"':
                        tt = 34;
                        break;
                    case 'r':
                        tt = 13;
                        break;
                    case 'f':
                        tt = 12;
                        break;
                    case 'v':
                        tt = 11;
                        break;
                    default:
                        tt = 0;
                        break;
	            }
	        }
	        rtn += tt;
	    }
	    return rtn;
	}
%}
%union
{
	scNStatements* statements;
	scNStatement* statement;
	scNDeclaration* declaration;
	scNVariableDeclaration* var_decl;
	scNFunctionDeclaration* func_decl;
	scNExpression* expression;
	scNNumber* number;
	scNDeclarationBody* decl_body;
	scNType* type;
	scNParams* param_list;
	scNExpressions* expressions;
	scNIdentifier* identifier;
	scNBlock* block;

	std::string* string;
	int token;
}

%token <string> TIDENTIFIER TINTEGER TDOUBLE TYSTRING TEXTERN TSTRING TCHAR
%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL TEXCLAMATION
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT TSEMICOLON TLBRACKET TRBRACKET TQUOTATION TYINT TYDOUBLE TYFLOAT TYCHAR TYBOOL TYVOID TBREAK TCONTINUE
%token <token> TPLUS TMINUS TMUL TDIV TAND TOR TXOR TMOD TNEG TNOT TSHIFTL TSHIFTR TWAVY TANDAND TOROR
%token <token> TIF TELSE TFOR TWHILE TRETURN TSTRUCT


%type <statements> statements program
%type <statement> statement if_statement if_else_statement while_statement for_statement return_statement fdefinition empty_statement
%type <var_decl> variable_decl
%type <func_decl> function_decl
%type <expression> expression unary_expression binary_expression reference_expression dereference_expression array_expression function_call assignment
%type <block> block
%type <identifier> identifier
%type <number> number
%type <decl_body> decl_body
%type <type> super_type
%type <param_list> param_list
%type <expressions> expressions
%type <token> bop uop type

%left TEQUAL
%left TANDAND TOROR
%left TCEQ TCNE TCLT TCLE TCGT TCGE
%left TPLUS TMINUS
%left TMUL TDIV TMOD

%start program

%%
program : statements { TopBlock = $1; }
		;

statements : statement { $$ = new scNStatements();
						 $$->statement_list.push_back(shared_ptr<scNStatement>($1)); }
		| statements statement { $1->statement_list.push_back(shared_ptr<scNStatement>($2)); }
		;

statement : variable_decl TSEMICOLON { $$ = $1; }
		| function_decl TSEMICOLON { $$ = $1; }
		| expression TSEMICOLON { $$ = $1; }
		| block { $$ = $1; }
		| if_statement { $$ = $1; }
		| if_else_statement { $$ = $1; }
		| while_statement { $$ = $1; }
		| for_statement { $$ = $1; }
		| return_statement TSEMICOLON { $$ = $1; }
		| fdefinition { $$ = $1; }
		| empty_statement { $$ = $1; }
		| TBREAK { $$ = new scNBreakStatement(); }
		| TCONTINUE { $$ = new scNContinueStatement(); }
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
		| TSTRING { $$ = new scNString(toRawString(*($1))); }
		| TCHAR { $$ = new scNChar(toRawString(*($1))); }
		;

variable_decl : type decl_body { $$ = new scNVariableDeclaration($1, shared_ptr<scNDeclarationBody>($2));}
		;

type    : TYINT { $$ = $1; }
		| TYDOUBLE { $$ = $1; }
		| TYFLOAT { $$ = $1; }
		| TYCHAR { $$ = $1; }
		| TYVOID { $$ = $1; }
		;

decl_body : TLPAREN decl_body TRPAREN { $$ = $2; }
		| TMUL decl_body { $$ = new scNDeclarationBody();
						   $$->name = $2->name;
						   $$->is_array = false;
						   $$->is_ptr = true;
						   $$->size = 1;
						   $$->p_size = $2->size;
						   $$->children = shared_ptr<scNDeclarationBody>($2); }
		| decl_body TLBRACKET TINTEGER TRBRACKET { $$ = new scNDeclarationBody();
												 $$->name = $1->name;
												 $$->is_array = true;
												 $$->is_ptr = false;
												 $$->size = $1->size * atol($3->c_str());
												 $$->children = shared_ptr<scNDeclarationBody>($1); }
		| identifier { $$ = new scNDeclarationBody();
					   $$->name = $1->name;
					   $$->is_array = false;
					   $$->is_ptr = false;
					   $$->size = 1;
					   $$->children = nullptr; }
		;

function_decl : type decl_body TLPAREN param_list TRPAREN { $$ = new scNFunctionDeclaration($1, shared_ptr<scNDeclarationBody>($2), shared_ptr<scNParams>($4)); }
		| type decl_body TLPAREN TRPAREN { $$ = new scNFunctionDeclaration($1, shared_ptr<scNDeclarationBody>($2), shared_ptr<scNParams>(new scNParams())); }
		| TEXTERN type decl_body TLPAREN param_list TRPAREN { $$ = new scNFunctionDeclaration($2, shared_ptr<scNDeclarationBody>($3), shared_ptr<scNParams>($5)); }
		| TEXTERN type decl_body TLPAREN TRPAREN { $$ = new scNFunctionDeclaration($2, shared_ptr<scNDeclarationBody>($3), shared_ptr<scNParams>(new scNParams())); }
		;

super_type : type { $$ = new scNType($1); }
		| super_type TMUL { $1->count += 1;
							$$ = $1; }
		;

param_list : variable_decl { $$ = new scNParams(); 
							 $$->param_list.push_back(shared_ptr<scNVariableDeclaration>($1)); }
		| param_list TCOMMA variable_decl { $1->param_list.push_back(shared_ptr<scNVariableDeclaration>($3)); 
											$$ = $1; }
		;

block : TLBRACE TRBRACE { $$ = new scNBlock(); }
		| TLBRACE statements TRBRACE { $$ = new scNBlock(); $$->statements = shared_ptr<scNStatements>($2); }
		;

if_statement : TIF TLPAREN expression TRPAREN statement { $$ = new scNIfStatement(shared_ptr<scNExpression>($3), shared_ptr<scNStatement>($5)); }
		;

if_else_statement : TIF TLPAREN expression TRPAREN statement TELSE statement { $$ = new scNIfElseStatement(shared_ptr<scNExpression>($3), shared_ptr<scNStatement>($5), shared_ptr<scNStatement>($7)); }
		;

while_statement : TWHILE TLPAREN expression TRPAREN statement { $$ = new scNWhileStatement(shared_ptr<scNExpression>($3), shared_ptr<scNStatement>($5)); }
		;

for_statement : TFOR TLPAREN expression TSEMICOLON expression TSEMICOLON expression TRPAREN statement { $$ = new scNForStatement(shared_ptr<scNExpression>($3), shared_ptr<scNExpression>($5), shared_ptr<scNExpression>($7), shared_ptr<scNStatement>($9)); }
		;

return_statement : TRETURN expression { $$ = new scNReturnStatement(shared_ptr<scNExpression>($2)); }
		| TRETURN { $$ = new scNReturnStatement(nullptr); }
		;

fdefinition : function_decl block { $$ = new scNFunctionDefinition(shared_ptr<scNFunctionDeclaration>($1), shared_ptr<scNBlock>($2)); }
		;

empty_statement : TSEMICOLON { $$ = new scNEmptyStatement(); }
		;

number : TINTEGER { $$ = new scNInt32Number(atol($1->c_str())); }
		| TDOUBLE { $$ = new scNDouble64Number(atof($1->c_str())); }
		;

assignment : expression TEQUAL expression { $$ = new scNAssignment(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3)); }
		;

identifier : TIDENTIFIER { $$ = new scNIdentifier(*($1)); }
		;

function_call : identifier TLPAREN expressions TRPAREN { $$ = new scNFunctionCall($1->name, shared_ptr<scNExpressions>($3)); }
		| identifier TLPAREN TRPAREN { $$ = new scNFunctionCall($1->name, shared_ptr<scNExpressions>(new scNExpressions())); }
		;

expressions : expression { $$ = new scNExpressions();
						   $$->expression_list.push_back(shared_ptr<scNExpression>($1)); }
		| expressions TCOMMA expression { $1->expression_list.push_back(shared_ptr<scNExpression>($3));
										  $$ = $1; }
		;

binary_expression : expression TCEQ expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TCNE expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TCLT expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TCLE expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TCGT expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TCGE expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TPLUS expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TMINUS expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TMUL expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TDIV expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TAND expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TOR expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TXOR expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TMOD expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TSHIFTR expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TSHIFTL expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TANDAND expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
		| expression TOROR expression { $$ = new scNBinaryExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3), $2); }
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
		| TANDAND { $$ = $1; }
		| TOROR { $$ = $1; }
		;

unary_expression : uop expression { $$ = new scNUnaryExpression(shared_ptr<scNExpression>($2), $1); }
		;

uop 	: TMINUS { $$ = $1; }
		| TEXCLAMATION { $$ = $1; }
		| TWAVY { $$ = $1; }
		;

reference_expression : TAND expression { $$ = new scNReferenceExpression(shared_ptr<scNExpression>($2)); }
		;

dereference_expression : TMUL expression { $$ = new scNDereferenceExpression(shared_ptr<scNExpression>($2)); }
		;

array_expression : expression TLBRACKET expression TRBRACKET { $$ = new scNArrayExpression(shared_ptr<scNExpression>($1), shared_ptr<scNExpression>($3)); }
		;

%%