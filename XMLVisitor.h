#include<iostream>
#include<vector>
using namespace std;
class XMLvisitor: public Visitor	{
	public:
		virtual void visit(ASTNode *e){
		}
		virtual void visit(ASTdecaf *e){
			xmlout<<"<"<<e->headerObj->IDname<<">"<<endl;
			if(e->headerObj)
				e->headerObj->accept(*this);								// koi zarurat?
			if(e->body_sectionObj)
				e->body_sectionObj->accept(*this);
			xmlout<<"</"<<e->headerObj->IDname<<">"<<endl;
		}
		virtual void visit(ASTheader *e){
		}
		virtual void visit(ASTbody_section *e){
			if(e->field_declObj)
				e->field_declObj->accept(*this);
			if(e->statement_declObj)
				e->statement_declObj->accept(*this);
		}
		virtual void visit(ASTfield_decl *e){	
			xmlout<<"\t<field_declarations count=\""<<(e->field_decl_lineObjs).size()<<"\">\n";
			for(vector<ASTfield_decl_line*>::iterator it=e->field_decl_lineObjs.begin(); it!=e->field_decl_lineObjs.end(); it++)	
				(*it)->accept(*this);
			xmlout<<"\t</field_declarations>\n";
		}
		virtual void visit(ASTfield_decl_line *e){
			if(e->id_decl_listObj)
				e->id_decl_listObj->accept(*this);
		}
		virtual void visit(ASTid_decl_list *e)	{
			for(vector<ASTid_decl *>::iterator it=e->id_declObjs.begin(); it!=e->id_declObjs.end(); it++)	
				(*it)->accept(*this);
		}
		virtual void visit(ASTid_decl *e)	{
			xmlout<<"\t\t<declaration name=\""<<e->idVal<<"\" "; 
			if(e->idDeclType==true)
				xmlout<<"count=\""<<e->sizeVal<<"\" ";
			if(strcmp(e->typeVal,"int")==0)
				xmlout<<"type=\"integer\" />\n";
			else
				xmlout<<"type=\"boolean\" />\n";
		}
		virtual void visit(ASTstatement_decl *e){
			xmlout<<"\t<statement_declarations count=\""<<(e->statementObjs).size()<<"\">\n";
			for(vector<ASTstatement*>::iterator it=e->statementObjs.begin(); it!=e->statementObjs.end(); it++)	
				(*it)->accept(*this);
			xmlout<<"\t</statement_declarations>\n";
		}
		virtual void visit(ASTstatement *e){
			if(e->statementType==false)
			{
				xmlout<<"\t\t<assignment>\n";
				if(e->locationObj)
					e->locationObj->accept(*this);													//	xmlout<<"\t\t\t<location id="<<e->idVal<<" />";
				if(e->exprObj)
					e->exprObj->accept(*this);														//	xmlout<<"\t\t\t<expr>";
				xmlout<<"\t\t</assignment>\n";
			}
			else if(e->statementType==true)
			{
				xmlout<<"\t\t<callout function=\""<<e->funcName<<"\">\n";
				if(e->callout_args_listObj!=NULL)
				{
					(e->callout_args_listObj)->accept(*this);
				}	
				xmlout<<"\t\t</callout>\n";
			}
		}
		virtual void visit(ASTcallout_args_list *e){
			for(vector<ASTcallout_arg *>::iterator it=e->callout_argObjs.begin(); it!=e->callout_argObjs.end(); it++)
				(*it)->accept(*this);
		}
		virtual void visit(ASTcallout_arg *e){
			xmlout<<"\t\t\t\""<<e->argName<<"\"\n";
		}
		virtual void visit(ASTexpr *e)	{
			switch(e->exprType)
			{
				case 1:		if(e->locationObj)
								e->locationObj->accept(*this);
							break;
				case 2:		xmlout<<"\t\t\t<integer value=\""<<e->intVal<<"\">"<<endl;	
							break;
				case 3:		xmlout<<"\t\t\t<boolean value=\"";
							e->boolVal?xmlout<<"true":xmlout<<"false";
							xmlout<<"\">"<<endl;		
							break;
				case 4:		xmlout<<"\t\t\t<character value=\""<<e->charVal<<"\">"<<endl;	
							break;
				case 5:		xmlout<<"\t\t\t<unary_expression type=\"not\">"<<endl;	
							if(e->exprObj)
								e->exprObj->accept(*this);
							xmlout<<"\t\t\t</unary_expression>"<<endl;	
							break;
				case 6:		xmlout<<"\t\t\t<unary_expression type=\"minus\">"<<endl;	
							if(e->exprObj)
								e->exprObj->accept(*this);
							xmlout<<"\t\t\t</unary_expression>"<<endl;	
							break;
				case 7:		
							xmlout<<"\t\t\t<binary_expression type=\"subtraction\">"<<endl;	
							if(e->exprObj1)
								e->exprObj1->accept(*this);
							if(e->exprObj2)
								e->exprObj2->accept(*this);
							xmlout<<"\t\t\t</binary_expression>"<<endl;	
							break;
				case 8:	
							xmlout<<"\t\t\t<binary_expression type=\"addition\">"<<endl;	
							if(e->exprObj1)
								e->exprObj1->accept(*this);
							if(e->exprObj2)
								e->exprObj2->accept(*this);
							xmlout<<"\t\t\t</binary_expression>"<<endl;	
							break;
				case 9:	
							xmlout<<"\t\t\t<binary_expression type=\"multiplication\">"<<endl;	
							if(e->exprObj1)
								e->exprObj1->accept(*this);
							if(e->exprObj2)
								e->exprObj2->accept(*this);
							xmlout<<"\t\t\t</binary_expression>"<<endl;	
							break;
				case 10:
							xmlout<<"\t\t\t<binary_expression type=\"division\">"<<endl;	
							if(e->exprObj1)
								e->exprObj1->accept(*this);
							if(e->exprObj2)
								e->exprObj2->accept(*this);
							xmlout<<"\t\t\t</binary_expression>"<<endl;	
							break;
				case 11:
							xmlout<<"\t\t\t<binary_expression type=\"remainder\">"<<endl;	
							if(e->exprObj1)
								e->exprObj1->accept(*this);
							if(e->exprObj2)
								e->exprObj2->accept(*this);
							xmlout<<"\t\t\t</binary_expression>"<<endl;	
							break;
				case 12:
							xmlout<<"\t\t\t<binary_expression type=\"";
							switch(e->binOpVal)	{
								case 1	:
									xmlout<<"less_than";
									break;
								case 2	:
									xmlout<<"greater_than";
									break;
								case 3	:
									xmlout<<"less_equal";
									break;
								case 4	:
									xmlout<<"greater_equal";
									break;
							}
							xmlout<<"\">"<<endl;	
							if(e->exprObj1)
								e->exprObj1->accept(*this);
							if(e->exprObj2)
								e->exprObj2->accept(*this);
							xmlout<<"\t\t\t</binary_expression>"<<endl;	
							break;
				case 13:
							xmlout<<"\t\t\t<binary_expression type=\"";
							switch(e->binOpVal)	{
								case 5	:
									xmlout<<"is_equal";
									break;
								case 6	:
									xmlout<<"is_not_equal";
									break;
							}
							xmlout<<"\">"<<endl;	
							if(e->exprObj1)
								e->exprObj1->accept(*this);
							if(e->exprObj2)
								e->exprObj2->accept(*this);
							xmlout<<"\t\t\t</binary_expression>"<<endl;	
							break;
				case 14:
							xmlout<<"\t\t\t<binary_expression type=\"";
							switch(e->binOpVal)	{
								case 7	:
									xmlout<<"and";
									break;
								case 8	:
									xmlout<<"or";
									break;
							}
							xmlout<<"\">"<<endl;	
							if(e->exprObj1)
								e->exprObj1->accept(*this);
							if(e->exprObj2)
								e->exprObj2->accept(*this);
							xmlout<<"\t\t\t</binary_expression>"<<endl;	
							break;
				case 15:
							if(e->exprObj)
								e->exprObj->accept(*this);
							break;
			}
		}
		virtual void visit(ASTlocation *e)	{
			if(e->locationType==false)
				xmlout<<"\t\t\t<location id=\""<<e->idVal<<"\" />\n";
			else
			{
				xmlout<<"\t\t\t<location id=\""<<e->idVal<<"\">"<<endl;
				xmlout<<"\t\t\t\t<position>"<<endl;
				if(e->exprObj)
					e->exprObj->accept(*this);
				xmlout<<"\t\t\t\t</position>"<<endl;
				xmlout<<"\t\t\t</location>"<<endl;
			}
		}
};	


