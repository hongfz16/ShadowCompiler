%{
#include <stdio.h>
#include <string>
#include "AST.hpp"
#include "grammar.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext)
#define TOKEN(t) ( yylval.token = t)
void mputs(char* s) {}
%}

%option noyywrap


%%
"#".*                   ;
[ \t\r\n]				;
"if"                    mputs("TIF"); return TOKEN(TIF);
"else"                  mputs("TELSE"); return TOKEN(TELSE);
"return"                mputs("TRETURN"); return TOKEN(TRETURN);
"for"                   mputs("TFOR"); return TOKEN(TFOR);
"while"                 mputs("TWHILE"); return TOKEN(TWHILE);
"struct"                mputs("TSTRUCT"); return TOKEN(TSTRUCT);
"int"                   mputs("TYINT");  return TOKEN(TYINT);
"double"                mputs("TYDOUBLE"); return TOKEN(TYDOUBLE);
"float"                 mputs("TYFLOAT"); return TOKEN(TYFLOAT);
"char"                  mputs("TYCHAR"); return TOKEN(TYCHAR);
"void"                  mputs("TYVOID"); return TOKEN(TYVOID);
"break"                 mputs("TBREAK"); return TOKEN(TBREAK);
"continue"              mputs("TCONTINUE"); return TOKEN(TCONTINUE);
"extern"                SAVE_TOKEN; mputs("TEXTERN"); return TEXTERN;
[a-zA-Z_][a-zA-Z0-9_]*	SAVE_TOKEN; mputs("TIDENTIFIER"); return TIDENTIFIER;
[0-9]+\.[0-9]*			SAVE_TOKEN; mputs("TDOUBLE"); return TDOUBLE;
[0-9]+  				SAVE_TOKEN; mputs("TINTEGER"); return TINTEGER;
\"(\\.|[^"])*\"         SAVE_TOKEN; mputs("TSTRING"); return TSTRING;
\'(\\.|[^'])\'          SAVE_TOKEN; mputs("TCHAR"); return TCHAR;
"="						mputs("TEQUAL"); return TOKEN(TEQUAL);
"=="					mputs("TCEQ"); return TOKEN(TCEQ);
"!="                    mputs("TCNE"); return TOKEN(TCNE);
"<"                     mputs("TCLT"); return TOKEN(TCLT);
"<="                    mputs("TCLE"); return TOKEN(TCLE);
">"                     mputs("TCGT"); return TOKEN(TCGT);
">="                    mputs("TCGE"); return TOKEN(TCGE);
"("                     mputs("TLPAREN"); return TOKEN(TLPAREN);
")"                     mputs("TRPAREN"); return TOKEN(TRPAREN);
"{"                     mputs("TLBRACE"); return TOKEN(TLBRACE);
"}"                     mputs("TRBRACE"); return TOKEN(TRBRACE);
"["                     mputs("TLBRACKET"); return TOKEN(TLBRACKET);
"]"                     mputs("TRBRACKET"); return TOKEN(TRBRACKET);
"."                     mputs("TDOT"); return TOKEN(TDOT);
","                     mputs("TCOMMA"); return TOKEN(TCOMMA);
"+"                     mputs("TPLUS"); return TOKEN(TPLUS);
"-"                     mputs("TMINUS"); return TOKEN(TMINUS);
"*"                     mputs("TMUL"); return TOKEN(TMUL);
"/"                     mputs("TDIV"); return TOKEN(TDIV);
"&"                     mputs("TAND"); return TOKEN(TAND);
"|"                     mputs("TOR"); return TOKEN(TOR);
"^"                     mputs("TXOR"); return TOKEN(TXOR);
"%"                     mputs("TMOD"); return TOKEN(TMOD);
">>"                    mputs("TSHIFTR"); return TOKEN(TSHIFTR);
"<<"                    mputs("TSHIFTL"); return TOKEN(TSHIFTL);
";"                     mputs("TSEMICOLON"); return TOKEN(TSEMICOLON);
"!"                     mputs("TEXCLAMATION"); return TOKEN(TEXCLAMATION);
"~"                     mputs("TWAVY"); return TOKEN(TWAVY);
"&&"                    mputs("TANDAND"); return TOKEN(TANDAND);
"||"                    mputs("TOROR"); return TOKEN(TOROR);
.						printf("Unknown token:%s\n", yytext); yyterminate();

%%