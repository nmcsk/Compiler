#include<iostream>
#include<vector>
using namespace std;

class Visitor{
	public:
		virtual void visit(ASTnode *e) = 0;
		virtual void visit(ASTdecaf *e) = 0;
		virtual void visit(ASTheader *e) = 0;
		virtual void visit(ASTbody_section *e) = 0;
		virtual void visit(ASTfield_decl *e) = 0;
		virtual void visit(ASTfield_decl_line *e) = 0;
		virtual void visit(ASTid_decl_list *e) = 0;
		virtual void visit(ASTid_decl *e) = 0;
		virtual void visit(ASTstatement_decl *e) = 0;
		virtual void visit(ASTstatement *e) = 0;
		virtual void visit(ASTcallout_args_list *e) = 0;
		virtual void visit(ASTcallout_arg *e) = 0;
		virtual void visit(ASTexpr *e) = 0;
		virtual void visit(ASTlocation *e) = 0;
};

void ASTnode::accept(Visitor &v){
	v.visit(this);
}

void ASTdecaf::accept(Visitor &v){
	v.visit(this);
}

void ASTheader::accept(Visitor &v){
	v.visit(this);
}

void ASTbody_section::accept(Visitor &v){
	v.visit(this);
}

void ASTfield_decl::accept(Visitor &v){
	v.visit(this);
}

void ASTfield_decl_line::accept(Visitor &v){
	v.visit(this);
}

void ASTid_decl_list::accept(Visitor &v){
	v.visit(this);
}

void ASTid_decl::accept(Visitor &v){
	v.visit(this);
}

void ASTstatement_decl::accept(Visitor &v){
	v.visit(this);
}

void ASTstatement::accept(Visitor &v){
	v.visit(this);
}

void ASTcallout_args_list::accept(Visitor &v){
	v.visit(this);
}

void ASTcallout_arg::accept(Visitor &v){
	v.visit(this);
}

void ASTexpr::accept(Visitor &v){
	v.visit(this);
}

void ASTlocation::accept(Visitor &v){
	v.visit(this);
}
