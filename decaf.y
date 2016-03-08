%{
#include<iostream>
#include<cstdio>
#include<fstream>
#include<string>
#include<string.h>
#include<vector>
#include<map>
#include<stack>
#include<llvm/Analysis/Verifier.h>
#include<llvm/Assembly/PrintModulePass.h>
#include<llvm/Bitcode/ReaderWriter.h>
#include<llvm/ExecutionEngine/GenericValue.h>
#include<llvm/ExecutionEngine/JIT.h>
#include<llvm/IR/Module.h>
#include<llvm/IR/Value.h>
#include<llvm/IR/IRBuilder.h>
#include<llvm/IR/Function.h>
#include<llvm/IR/Type.h>
#include<llvm/IR/DerivedTypes.h>
#include<llvm/IR/LLVMContext.h>
#include<llvm/IR/Instructions.h>
#include<llvm/IR/CallingConv.h>
#include<llvm/Support/TargetSelect.h>
#include<llvm/Support/raw_ostream.h>
#include<llvm/PassManager.h>
#include "ast.h"
#include "Visitor.h"
#include "XMLVisitor.h"
#include "CodeGenVisitor.h"
ofstream xmlout("XML_Visitor.txt")

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" char * yytext;
extern int lineNo;

using namespace std;

ASTnode *ASTroot;

void yyerror(const char *s);
%}

%union{
	int ival;
	char cval;
	bool bval;
	char* sval;
	ASTnode *astNodeT;
	ASTdecaf *astDecafT;
	ASTheader *astHeaderT;
	ASTbody_section *astBodySectionT;
	ASTfield_decl *astFieldDeclT;
	ASTfield_decl_line *astFieldDeclLineT;
	ASTid_decl_list *astIdDeclListT;
	ASTid_decl *astIdDeclT;
	ASTstatement_decl *astStatementDeclT;
	ASTstatement *astStatementT;
	ASTcallout_args_list *astCalloutArgsListT;
	ASTcallout_arg *astCalloutArgT;
	ASTlocation *astLocationT;
	ASTexpr astExprT;
}

%token CLASS
%token CALLOUT
%token OPEN_BRACE
%token CLOSE_BRACE
%token OPEN_PARAN
%token CLOSE_PARAN
%token OPEN_SQR_BRACKET
%token CLOSE_SQR_BRACKET
%token NOT
%token MINUS
%token PLUS
%token MULTIPLY
%token DIVIDE
%token COMMA
%token SEMICOLON
%token MOD

%token <sval> ASSIGN_OP;
%token <sval> REL_OP;
%token <sval> EQ_OP;
%token <sval> COND_OP;
%token <sval> TYPE;
%token <bval> BOOL_LITERAL;
%token <ival> INT_LITERAL;
%token <cval> CHAR_LITERAL;
%token <sval> STRING_LITERAL;
%token <sval> ID;
%token <sval> UNKNOWN_CHAR;

%left REL_OP COND_OP EQ_OP
%left PLUS MINUS
%left MULTIPLY DIVIDE MOD
%left NOT
%left NEG_OP

%type <astDecafT> decaf;
%type <astHeaderT> header;
%type <astBodySectionT> body_section;
%type <astFieldDeclT> field_decl;
%type <astFieldDeclLineT> field_decl_line;
%type <astIdDeclListT> id_decl_list;
%type <astIdDeclT> id_decl;
%type <astStatementDeclT> statement_decl;
%type <astStatementT> statement;
%type <astCalloutArgsListT> callout_args_list;
%type <astCalloutArgT> callout_arg;
%type <astLocationT> location;
%type <astExprT> expr;


%%
decaf: header body_section { $$ = new ASTdecaf($1,$2); ASTroot = $$; }
;
header: CLASS ID { $$ = new ASTheader($2);}
;
body_section: OPEN_BRACE field_decl statement_decl CLOSE_BRACE { $$ = new ASTbody_section($2,$3);}
	| OPEN_BRACE field_decl CLOSE_BRACE { $$ = new ASTbody_section($2);}
	| OPEN_BRACE statement_decl CLOSE_BRACE { $$ = new ASTbody_section($2);}
	| OPEN_BRACE CLOSE_BRACE { $$ = new ASTbody_section();}
	;
field_decl: field_decl field_decl_line { $1->field_decl_lineObjs.push_back($2); $$ = $1;}
	| field_decl_line { $$ = new ASTfield_decl(); $$->field_decl_lineObjs.push_back($1);}
	;
field_decl_line: TYPE id_decl_list SEMICOLON { $$ = new ASTfield_decl_line($1,$2); 
			 for(vector<ASTid_decl*>::iterator it=$2->id_declObjs.begin(); it!=$2->id_declObjs.end(); it++)
				 (*it)->typeVal=strdup($1);
		 }
;
id_decl_list: id_decl_list COMMA id_decl {$1->id_declObjs.push_back($3); $$ = $1; }
	| id_decl { $$ = new ASTid_decl_list() ; $$->id_declObjs.push_back($1); }
	;
id_decl: ID { $$ = new ASTid_decl($1); }
	| ID OPEN_SQR_BRACKET INT_LITERAL CLOSE_SQR_BRACKET { $$ = new ASTid_decl($1,$3);}
	;
statement_decl: statement_decl statement { $1->statementObjs.push_back($2); $$ = $1;}
	| statement { $$ = new ASTstatement_decl(); $$->statementObjs.push_back($1);}
	;
statement: location ASSIGN_OP expr SEMICOLON { $$ = new ASTstatement($1,$2,$3);}
	| CALLOUT OPEN_PARAN STRING_LITERAL CLOSE_PARAN SEMICOLON { $$ = new ASTstatement($3); }
	| CALLOUT OPEN_PARAN STRING_LITERAL COMMA callout_args_list CLOSE_PARAN SEMICOLON { $$ = new ASTstatement($3,$5);}
	;
callout_args_list: callout_args_list COMMA callout_arg { $1->callout_argObjs.push_back($3);}
	| callout_arg { $$ = new ASTcallout_args_list(); $$->callout_argObjs.push_back($1);}
	;
callout_arg: STRING_LITERAL { $$ = new ASTcallout_arg($1);}
	| expr { $$ = new ASTcallout_arg($1);}
	;
location: ID { $$ = new ASTlocation($1);}
	| ID OPEN_SQR_BRACKET expr CLOSE_SQR_BRACKET { $$ = new ASTlocation($1,$3);}
	;
expr: location { $$ = new ASTexpr($1);}
	| INT_LITERAL { $$ = new ASTexpr($1);}
	| BOOL_LITERAL { $$ = new ASTexpr($1);}
	| CHAR_LITERAL { $$ = new ASTexpr($1);}
	| NOT expr { $$ = new ASTexpr(5,$2);}
	| MINUS expr %prec NEG_OP {$$ = new ASTexpr(6,$2);}
	| expr MINUS expr {$$ = new ASTexpr($1,7,$3);}
	| expr PLUS expr  {$$ = new ASTexpr($1,8,$3);}
	| expr MULTIPLY expr  {$$ = new ASTexpr($1,9,$3);}
	| expr DIVIDE expr {$$ = new ASTexpr($1,10,$3);}
	| expr MOD expr  {$$ = new ASTexpr($1,11,$3);}
	| expr REL_OP expr  {$$ = new ASTexpr($1,$2,$3,12);}
	| expr EQ_OP expr  {$$ = new ASTexpr($1,$2,$3,13);}
	| expr COND_OP expr  {$$ = new ASTexpr($1,$2,$3,14);}
	| OPEN_PARAN expr CLOSE_PARAN  {$$ = new ASTexpr($2);}
	;
%%

int main(int argc, char **argv){
	FILE* myfile = fopen(argv[1],"r");
	if(!myfile){
		cout<<"File can't be opened"<<endl;
		return -1;
	}
	yyin = myfile;
	cout<<"201330072\n201201062\n";
	int parseFlag;
	do{
		parseFlag=yyparse();
	}while(!feof(yyin));
	if(parseFlag==0){
		cout<<"Success\n";
		XMLvisitor xv;
		ASTroot->accept(xv);
		CodeGenVisitor cgv;
		ASTroot->accept(cgv);
	}
	else if(parseFlag==1)
		cout<<"Syntax Error\n";
	xmlout.close();
}
void yyerror(const char *s){
}
