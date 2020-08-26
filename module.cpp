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

Function *createFunc(IRBuilder<> &Builder, std::string Name) {
	std::vector<Type*> Integers(FunArgs.size(), Type::getInt32Ty(TheContext));
	FunctionType *funcType = FunctionType::get(Builder.getInt32Ty(), Integers, false);
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
int main ( int argc, char *argv[] )
{
	FunArgs.push_back("a");
	FunArgs.push_back("b");
	
	llvm::LLVMContext &Context = TheContext;
	TheModule = new Module("my compiler", Context);
	
	GlobalVariable *gvar = createGlob(Builder, "x");
	Function *fooFunc = createFunc(Builder, "foo");
	setFuncArgs(fooFunc, FunArgs);
	BasicBlock *entry = createBB(fooFunc, "entry");
	
	Builder.SetInsertPoint(entry);
	Builder.CreateRet(Builder.getInt32(0));
	
	verifyFunction(*fooFunc);
	TheModule->print(llvm::outs(), nullptr);
	return 0;

}
