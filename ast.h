#include<vector>
#include<iostream>
#include<string.h>
#include<string>
#include<llvm/IR/Value.h>
using namespace std;
class ASTnode;
class ASTdecaf;
class ASTheader;
class ASTbody_section;
class ASTfield_decl;
class ASTfield_decl_line;
class ASTid_decl_list;
class ASTid_decl;
class ASTstatement_decl;
class ASTstatement;
class ASTcallout_args_list;
class ASTcallout_arg;
class ASTexpr;
class ASTlocation;

class ASTnode{
	public:
		ASTdecaf* decafObj;
		llvm::Value* retVal;
		ASTnode(){
		}
		virtual void accept(class Visitor &v) = 0;
};

class ASTdecaf: public ASTnode{
	public:
		ASTheader *headerObj;
		ASTbody_section *body_sectionObj;
		llvm::Value* retVal;
		ASTdecaf(){
		}
		ASTdecaf(ASTheader *p1, ASTbody_section *p2){
			this->headerObj = p1;
			this->body_sectionObj = p2;
		}
		virtual void accept(Visitor &v);
};

class ASTheader: public ASTdecaf{
	public:
		char* IDname;
		llvm::Value* retVal;
		ASTheader(){
		}
		ASTheader(char *p){
			this->IDname = p;
		}
		virtual void accept(Visitor &v);
};

class ASTbody_section: public ASTdecaf{
	public:
		ASTfield_decl *field_declObj;
		ASTstatement_decl *statement_declObj;
		llvm::Value* retVal;
		ASTbody_section(){
		}
		ASTbody_section(ASTfield_decl *p){
			this->field_declObj = p;
		}
		ASTbody_section(ASTstatement_decl *p){
			this->statement_declObj = p;
		}
		ASTbody_section(ASTfield_decl *p1, ASTstatement_decl *p2){
			this->field_declObj = p1;
			this->statement_declObj = p2;
		}
		virtual void accept(Visitor &v);
};

class ASTfield_decl: public ASTbody_section{
	public:
		vector<ASTfield_decl_line*> field_decl_lineObjs;
		llvm::Value* retVal;
		ASTfield_decl(){
		}
		virtual void accept(Visitor &v);
};

class ASTfield_decl_line: public ASTfield_decl{
	public:
		ASTid_decl_list* id_decl_listObj;
		llvm::Value* retVal;
		ASTfield_decl_line(){
		}
		ASTfield_decl_line(char *p1, ASTid_decl_list* p2){
			this->id_decl_listObj = p2;
		}
		virtual void accept(Visitor &v);
};

class ASTid_decl_list: public ASTfield_decl_line{
	public:
		vector<ASTid_decl*>id_declObjs;
		llvm::Value* retVal;
		ASTid_decl_list(){
		}
		virtual void accept(Visitor &v);
};

class ASTid_decl: public ASTid_decl_list{
	public:
		bool idDeclType;
		char *typeVal;
		char *idVal;
		int sizeVal;
		llvm::Value* retVal;
		ASTid_decl(){
		}
		ASTid_decl(char *p){
			this->idVal = strdup(p);
		}
		ASTid_decl(char *p1, int p2){
			this->idVal = strdup(p1);
			this->sizeVal = p2;
		}
		virtual void accept(Visitor &v);
};

class ASTstatement_decl: public ASTbody_section{
	public:
		vector<ASTstatement*>statementObjs;
		llvm::Value* retVal;
		ASTstatement_decl(){
		}
		virtual void accept(Visitor &v);
};

class ASTstatement: public ASTstatement_decl{
	public:
		bool statementType;
		ASTlocation *locationObj;
		char *assignOpVal;
		ASTexpr *exprObj;
		char *funcName;
		ASTcallout_args_list *callout_args_listObj;
		llvm::Value* retVal;
		ASTstatement(){
		}
		ASTstatement(ASTlocation *p1, char *p2, ASTexpr *p3){
			this->statementType=false;
			this->locationObj = p1;
			this->assignOpVal = strdup(p2);
			this->exprObj = p3;
		}
		ASTstatement(char *p){
			this->statementType=true;
			this->funcName = strdup(p);
		}
		ASTstatement(char *p1, ASTcallout_args_list *p2){
			this->statementType=true;
			this->funcName = strdup(p1);
			this->callout_args_listObj = p2;
		}
		virtual void accept(Visitor &v);
};

class ASTcallout_args_list: public ASTstatement{
	public:
		vector<ASTcallout_arg *> callout_argObjs;
		llvm::Value* retVal;
		ASTcallout_args_list(){
		};
		virtual void accept(Visitor &v);
};

class ASTcallout_arg: public ASTcallout_args_list{
	public:
		char *argName;
		ASTexpr *exprObj;
		llvm::Value* retVal;
		ASTcallout_arg(){
		}
		ASTcallout_arg(char *p){
			this->argName = strdup(p);
		}
		ASTcallout_arg(ASTexpr *p){
			this->exprObj = p;
		}
		virtual void accept(Visitor &v);
};

class ASTexpr: public ASTstatement{
	public:
		int exprType;
		ASTlocation *locationObj;
		int intVal;
		bool boolVal;
		char charVal;
		ASTexpr *exprObj;
		ASTexpr *exprObj1;
		ASTexpr *exprObj2;
		int binOpVal;
		llvm::Value* retVal;
		ASTexpr(){
		}
		ASTexpr(ASTlocation *p){
			this->exprType = 1;
			this->locationObj = p;
		}
		ASTexpr(int p){
			this->exprType = 2;
			this->intVal = p;
		}
		ASTexpr(bool p){
			this->exprType = 3;
			this->boolVal = p;
		}
		ASTexpr(char p){
			this->exprType = 4;
			this->charVal = p;
		}
		ASTexpr(int p1, ASTexpr *p2){
			this->exprType = p1;
			this->exprObj = p2;
		}
		ASTexpr(ASTexpr *p1, int p2, ASTexpr *p3){
			this->exprObj1 = p1;
			this->exprType = p2;
			this->exprObj2 = p3;
		}
		ASTexpr(ASTexpr *p1, char *p2, ASTexpr *p3, int p4){
			this->exprObj1 = p1;
			if(strcmp(p2,"<")==0)
				this->binOpVal = 1;
			else if(strcmp(p2,">")==0)
				this->binOpVal = 2;
			else if(strcmp(p2,"<=")==0)
				this->binOpVal = 3;
			else if(strcmp(p2,">=")==0)
				this->binOpVal = 4;
			else if(strcmp(p2,"==")==0)
				this->binOpVal = 5;
			else if(strcmp(p2,"!=")==0)
				this->binOpVal = 6;
			else if(strcmp(p2,"&&")==0)
				this->binOpVal = 7;
			else if(strcmp(p2,"||")==0)
				this->binOpVal = 8;
			this->exprObj2 = p3;
			this->exprType = p4;
		}
		ASTexpr(ASTexpr *p){
			this->exprType = 15;
			this->exprObj = p;
		}
		virtual void accept(Visitor &v);
};

class ASTlocation: public ASTstatement{
	public:
		bool locationType;
		char *typeVal;
		char *idVal;
		ASTexpr *exprObj;
		llvm::Value* retVal;
		ASTlocation(){
		}
		ASTlocation(char *p1){
			this->locationType=false;
			this->idVal = strdup(p1);
		}
		ASTlocation(char *p1, ASTexpr *p2){
			this->locationType=true;
			this->idVal = strdup(p1);
			this->exprObj = p2;
		}
		virtual void accept(Visitor &v);
};
