#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;
static IRBuilder<> Builder(getGlobalContext());

class CodeGenVisitor: public Visitor{
	public:
		struct cmp_str{
			bool operator()(char const *a, char const *b){
				return std::strcmp(a,b)<0;
			}
		};
		Module *module;
		std::map<char *, Value*, cmp_str> symbolTable;
		Function *mainFunction;
		BasicBlock *bblock;
		CodeGenVisitor(){
			this->module = new Module("main",getGlobalContext());
		}
		static Type *typeof(char * type){
			if(strcmp(type,"int")==0)
				return TYPE::getInt64Ty(getGlobalContext());
			else if(strcmp(type,"boolean")==0)
				return TYPE::getInt1Ty(getGlobalContext());
			return TYPE::getVoidTy(getGlobalContext());
		}
		virtual void visit(ASTnode *e){
			e->retVal = NULL;
		}
		virtual void visit(ASTdecaf *e){
			vector<Type*>argTypes;
			FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()), argTypes, false);
			mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "Class Program", this->module);
			bblock = BasicBlock::Create(getGlobalContext(), "entry", mainFunction, 0);
			e->headerObj->accept(*this);
			e->body_sectionObj->accept(*this);
			e->retVal = NULL;
			PassManager pm;
			pm.add(createPrintModulePass(&outs()));
			pm.run(*module);
		}
		virtual void visit(ASTheader *e){
			e->retVal = NULL;
		}
		virtual void visit(ASTbody_section *e){
			e->field_declObj->accept(*this);
			e->statement_declObj->accept(*this);
			e->retVal = NULL;
		}
		virtual void visit(ASTfield_decl *e){
			for(vector<ASTfield_decl_line*>::iterator it=e->field_decl_lineObjs.begin(); it!=e->field_decl_lineObjs.end();it++)
				(*it)->accept(*this);
			e->retVal=NULL;
		}
		virtual void visit(ASTfield_decl_line *e){
			e->id_decl_listObj->accept(*this);
			e->retVal=NULL;
		}
		virtual void visit(ASTid_decl_list* e){
			for(vector<ASTid_decl*>::iterator it=e->id_declObjs.begin(); it!=e->id_declObjs.end(); it++)
				(*it)->accept(*this);
			e->retVal=NULL;
		}
		virtual void visit(ASTid_decl *e){
			if(e->idDeclType==false){
				AllocaInst *alloc = new AllocaInst(typeOf(e->typeVal), sizeV, e->idVal, this->bblock);
				this->symbolTable[e->idVal] = alloc;
				e->retVal = alloc;
			}
			else{
				Value* sizeV = ConstantInt::get(Type::getInt64Ty(getGlobalContext()), e->sizeVal, true);
				AllocaInst *alloc = new AllocaInst(typeOf(e->typeVal), sizeV, e->idVal, this->bblock);
				this->symbolTable[e->idVal] = alloc;
				e->retVal = alloc;
			}
		}
		virtual void visit(ASTstatement_decl *e){
			for(vector<ASTstatement*>::iterator it=e->statementObjs.begin(); it!=e->statementObjs.end(); it++)
				(*it)->accept(*this);
			e->retVal = NULL;
		}
		virtual void visit(ASTstatement *e){
			e->retVal=NULL;
			if(e->statementType==false){
				e->locationObj->accept(*this);
				e->exprObj->accept(*this);
				if(e->locationObj->retVal!=NULL && e->exprObj->retVal!=NULL)
					e->retVal = new StoreInst(e->exprObj->retVal, e->locationObj->retVal, false, this->bblock);
			}
			else if(e->statementType==true){
				if(e->callout_args_listObj!=NULL)
					e->callout_args_listObj->accept(*this);
				e->retVal = NULL;
			}
		}
		virtual void visit(ASTcallout_args_list* e){
			e->retVal = NULL;
			for(vector<ASTcallout_arg*>::iterator it=e->callout_argObjs.begin(); it!=e->callout_argObjs.end(); it++)
				(*it)->accept(*this);
		}
		virtual void visit(ASTcallout_arg *e){
			e->retVal = NULL;
		}
		virtual void visit(ASTexpr *e){
			e->retVal=NULL;
			switch(e->exprType){
				case 1: e->locationObj->accept(*this);
					e->retVal = e->locationObj->retVal;
					break;
				case 2: e->retval=ConstantInt::get(Type::getInt64Ty(getGlobalContext()),e->intVal, true);
					break;
				case 3: e->retval=ConstantInt::get(Type::getInt64Ty(getGlobalContext()),(int)e->boolVal, true);
					break;
				case 4: e->retval=ConstantInt::get(Type::getInt64Ty(getGlobalContext()),(int)e->charVal, true);
					break;
				case 5:	e->exprObj->accept(*this);
					e->retVal = Builder.CreateNot(e->exprObj->retVal, "notInst");
					break;
				case 6:	e->exprObj->accept(*this);										
					e->retVal = Builder.CreateFNeg(e->exprObj->retVal, "negInst");
					break;
				case 7:	e->exprObj1->accept(*this);															
					e->exprObj2->accept(*this);
					e->retVal = Builder.CreateFSub(e->exprObj1->retVal, e->exprObj2->retVal, "subInst");
					break;
				case 8:	e->exprObj1->accept(*this);															
					e->exprObj2->accept(*this);
					e->retVal = Builder.CreateFAdd(e->exprObj1->retVal, e->exprObj2->retVal, "addInst");
					break;
				case 9:	e->exprObj1->accept(*this);																
					e->exprObj2->accept(*this);
					e->retVal = Builder.CreateFMul(e->exprObj1->retVal, e->exprObj2->retVal, "mulInst");
					break;
				case 10:e->exprObj1->accept(*this);															
					e->exprObj2->accept(*this);
					e->retVal = Builder.CreateFDiv(e->exprObj1->retVal, e->exprObj2->retVal, "divInst");
					break;
				case 11:e->exprObj1->accept(*this);														
					e->exprObj2->accept(*this);
					e->retVal = Builder.CreateFRem(e->exprObj1->retVal, e->exprObj2->retVal, "modInst");
					break;
				case 12:e->exprObj1->accept(*this);															
					e->exprObj2->accept(*this);
					switch(e->binOpVal){
						case 1: e->retVal = Builder.CreateICmpSLT(e->exprObj1->retVal, e->exprObj2->retVal, "ltCmp");
						        break;
						case 2:	e->retVal = Builder.CreateICmpSGT(e->exprObj1->retVal, e->exprObj2->retVal, "gtCmp");
							break;
						case 3:	e->retVal = Builder.CreateICmpSLE(e->exprObj1->retVal, e->exprObj2->retVal, "leCmp");
							break;
						case 4:	e->retVal = Builder.CreateICmpSGE(e->exprObj1->retVal, e->exprObj2->retVal, "geCmp");
							break;
					}
					break;
				case 13:e->exprObj1->accept(*this);																
					e->exprObj2->accept(*this);
					switch(e->binOpVal){
						case 5:	e->retVal = Builder.CreateICmpEQ(e->exprObj1->retVal, e->exprObj2->retVal, "eqCmp");
							break;
						case 6:	e->retVal = Builder.CreateICmpNE(e->exprObj1->retVal, e->exprObj2->retVal, "neqCmp");
							break;
					}
					break;
				case 14:e->exprObj1->accept(*this);																
					e->exprObj2->accept(*this);
					switch(e->binOpVal){
						case 7:	e->retVal = Builder.CreateAnd(e->exprObj1->retVal, e->exprObj2->retVal, "andCmp");
							break;
						case 8:	e->retVal = Builder.CreateOr(e->exprObj1->retVal, e->exprObj2->retVal, "orCmp");
							break;
					}
					break;
				case 15:e->exprObj->accept(*this);															
					e->retVal = e->exprObj->retVal;
					break;
			}
		}
		virtual void visit(ASTlocation *e){
			e->retVal=NULL;
			if(e->locationType==false){
				if(this->symbolTable.find(e->idVal)!=this->symbolTable.end()){
					e->retVal = this->symbolTable[e->idVal];	
				}	
			}
			else{
				if(this->symbolTable.find(e->idVal)!=this->symbolTable.end()){
					e->retVal = this->symbolTable[e->idVal];
					if(e->exprObj)
						e->exprObj->accept(*this);
				}
			}
		}
};
