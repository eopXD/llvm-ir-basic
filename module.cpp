#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Verifier.h"

#include <vector>
#include <string>
using namespace llvm;

static Module *TheModule;
static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::vector<std::string> FunArgs;
typedef SmallVector<BasicBlock*, 16> BBList;
typedef SmallVector<Value*, 16> ValList;

Function *createFunc(IRBuilder<> &Builder, std::string Name) {
	Type *vecTy = VectorType::get(Type::getInt32Ty(TheContext), 2);
	Type *ptrTy = vecTy->getPointerTo(0);
	FunctionType *funcType = FunctionType::get(Builder.getInt32Ty(), ptrTy, false);
	Function *fooFunc = Function::Create(
		funcType, llvm::Function::ExternalLinkage, Name, TheModule);
	return fooFunc;
}
void setFuncArgs ( Function *fooFunc, std::vector<std::string> FunArgs ) {
	unsigned Idx = 0;
	Function::arg_iterator AI, AE;
	for ( AI = fooFunc->arg_begin(), AE = fooFunc->arg_end(); AI != AE; ++AI, ++Idx ) {
		AI->setName(FunArgs[Idx]);
	}
}
BasicBlock* createBB ( Function *fooFunc, std::string Name ) {
	return BasicBlock::Create(TheContext, Name, fooFunc);
}
GlobalVariable *createGlob ( IRBuilder<> &Builder, std::string Name ) {
	TheModule->getOrInsertGlobal(Name, Builder.getInt32Ty());
	GlobalVariable *gvar = TheModule->getNamedGlobal(Name);
	gvar->setLinkage(GlobalVariable::CommonLinkage);
	gvar->setAlignment((MaybeAlign)4);
	return gvar;
}

Value* createArith ( IRBuilder<> &Builder, Value *L, Value *R ) {
	return Builder.CreateMul(L, R, "multmp");
}

Value* CreateIfElse ( IRBuilder<> &Builder, BBList List, ValList VL ) {
	Value *Condtn = VL[0];
	Value *Arg1 = VL[1];
	BasicBlock *ThenBB = List[0];
	BasicBlock *ElseBB = List[1];
	BasicBlock *MergeBB = List[2];
	Builder.CreateCondBr(Condtn, ThenBB, ElseBB);

	Builder.SetInsertPoint(ThenBB);
	Value *ThenVal = Builder.CreateAdd(Arg1, Builder.getInt32(1));
	Builder.CreateBr(MergeBB);

	Builder.SetInsertPoint(ElseBB);
	Value *ElseVal = Builder.CreateAdd(Arg1, Builder.getInt32(2));
	Builder.CreateBr(MergeBB);

	unsigned PhiBBSize = List.size() - 1;
	Builder.SetInsertPoint(MergeBB);
	PHINode *Phi = Builder.CreatePHI(Type::getInt32Ty(TheContext), PhiBBSize, "iftmp");
	Phi->addIncoming(ThenVal, ThenBB);
	Phi->addIncoming(ElseVal, ElseBB);

	return Phi;
}

Value* CreateLoop ( IRBuilder<> &Builder, BBList List, ValList VL,
	Value *StartVal, Value *EndVal ) {
	BasicBlock *PreheaderBB = Builder.GetInsertBlock();
	Value *val = VL[0];
	BasicBlock *LoopBB = List[0];
	Builder.CreateBr(LoopBB);
	Builder.SetInsertPoint(LoopBB);

	PHINode *IndVar = Builder.CreatePHI(Type::getInt32Ty(TheContext), 2, "i");
	IndVar->addIncoming(StartVal, PreheaderBB);
	
	Value *Add = Builder.CreateAdd(val, Builder.getInt32(5), "addtmp");
	Value *StepVal = Builder.getInt32(1);
	Value *NextVal = Builder.CreateAdd(IndVar, StepVal, "nextval");
	Value *EndCond = Builder.CreateICmpULT(IndVar, EndVal, "endcond");
	EndCond = Builder.CreateICmpNE(EndCond, Builder.getInt1(0), "loopcond");
	
	BasicBlock *LoopEndBB = Builder.GetInsertBlock();
	BasicBlock *AfterBB = List[1];
	
	Builder.CreateCondBr(EndCond, LoopBB, AfterBB);
	Builder.SetInsertPoint(AfterBB);
	
	IndVar->addIncoming(NextVal, LoopEndBB);
	return Add;
}
Value* getElemPtr ( IRBuilder<> &Builder, Value *Base, Value *Offset ) {
	return Builder.CreateGEP(Builder.getInt32Ty(), Base, Offset, "a1");
}
Value* getLoad ( IRBuilder<> &Builder, Value *Addr ) {
	return Builder.CreateLoad(Addr, "load");
}
int main ( int argc, char *argv[] )
{
	FunArgs.push_back("a");
	FunArgs.push_back("b");
	
	llvm::LLVMContext &Context = TheContext;
	TheModule = new Module("my compiler", Context);
	
	//GlobalVariable *gvar = createGlob(Builder, "x");
	Function *fooFunc = createFunc(Builder, "foo");
	setFuncArgs(fooFunc, FunArgs);
	BasicBlock *entry = createBB(fooFunc, "entry");
	
	Builder.SetInsertPoint(entry);
	
/*	Value *Arg1 = static_cast<Value*>(fooFunc->arg_begin());
	Value *constant = Builder.getInt32(16);
	Value *val1 = createArith(Builder, Arg1, constant);
	Value *val2 = Builder.getInt32(100);
		
	Value *Compare = Builder.CreateICmpULT(val1, val2, "cmptmp");
	//Compare->printAsOperand(outs());
	Value *Condtn = Builder.CreateICmpNE(Compare, Builder.getInt1(0), "ifcond");

	ValList VL;
	VL.push_back(Condtn);
	VL.push_back(Arg1);

	BasicBlock *ThenBB = createBB(fooFunc, "then");
	BasicBlock *ElseBB = createBB(fooFunc, "else");
	BasicBlock *MergeBB = createBB(fooFunc, "merge");
	BBList List;
	List.push_back(ThenBB);
	List.push_back(ElseBB);
	List.push_back(MergeBB);

	Value *v = CreateIfElse(Builder, List, VL);
	Builder.CreateRet(v);*/

/*	Function::arg_iterator AI = fooFunc->arg_begin();
	Value *Arg1 = AI++;
	Value *Arg2 = AI;
	Value *constant = Builder.getInt32(16);
	Value *val = createArith(Builder, Arg1, constant);
	ValList VL;
	VL.push_back(Arg1);

	BBList List;
	BasicBlock *LoopBB = createBB(fooFunc, "loop");
	BasicBlock *AfterBB = createBB(fooFunc, "afterloop");
	List.push_back(LoopBB);
	List.push_back(AfterBB);

	Value *StartVal = Builder.getInt32(1);
	Value *Res = CreateLoop(Builder, List, VL, StartVal, Arg2);
	Builder.CreateRet(Res);*/
	Value *Base = fooFunc->arg_begin();
	Value *gep = getElemPtr(Builder, Base, Builder.getInt32(1));
	Value *load = getLoad(Builder, gep);

	verifyFunction(*fooFunc);
	TheModule->print(outs(), nullptr);
	return 0;

}
