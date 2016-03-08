%{
#include<cstdio>
#include<iostream>
#include<fstream>
using namespace std;
#include "ast.h"
#include "decaf.tab.h"
#define YY_DECL extern "C" int yylex()
int lineNo = 1;
%}

%%
[ \t]
\n {lineNo++;}
class {return CLASS; }
callout {return CALLOUT;}
\{ { return OPEN_BRACE;}
\} {return CLOSE_BRACE;}
\( {return OPEN_PARAN;}
\) {return CLOSE_PARAN;}
[\[] { return OPEN_SQR_BRACKET;}
\] { return CLOSE_SQR_BRACKET;}
! { return NOT;}
\- {return MINUS;}
\+ {return PLUS;}
\* {return MULTIPLY;}
\/ {return DIVIDE;}
, {return COMMA;}
; { return SEMICOLON;}
% {return MOD;}
(=\+=\-=) {yylval.sval = strdup(yytext); return ASSIGN_OP;}
(<|>|<=|>=) {yylval.sval  = strdup(yytext); return REL_OP;}
(==|!=) {yylval.sval = strdup(yytext); return EQ_OP;}
(&&|\|\|) {yylval.sval = strdup(yytext); return COND_OP;}
(int|boolean) {yylval.sval=strdup(yytext); return TYPE;}
(true|false) {
	if(yytext[0] == 't') 
		yylval.bval = true;
	else 
		yylval.bval = false;
	return BOOL_LITERAL;
}
[0-9]+ {yylval.ival = strtol(yytext, NULL, 10); return INT_LITERAL;}
0(x|X)[a-fA-F0-9]+ {yylval.ival = strtol(yytext,NULL,16); return INT_LITERAL;}
'[ -~]' {yylval.cval = yytext[1]; return CHAR_LITERAL;}
\"[ -!#-~]*\" { 
	char * temp = strdup(yytext);
	int i;
	for(i=0;i<strlen(temp) -2;i++)
		temp[i] = temp[i+1];
	temp[i] = '\0';
	yylval.sval=temp;
	return STRING_LITERAL;
}
[a-zA-Z][a-zA-Z0-9]* {yylval.sval=strdup(yytext); return ID;}
. {return UNKNOWN_CHAR;}
%%
