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


Function *createFunc(IRBuilder<> &Builder, std::string Name) {
	FunctionType *funcType = llvm::FunctionType::get(Builder.getInt32Ty(), false);
	Function *fooFunc = llvm::Function::Create(
	funcType, llvm::Function::ExternalLinkage, Name, TheModule);
	return fooFunc;
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
	llvm::LLVMContext &Context = TheContext;
	TheModule = new Module("my compiler", Context);
	GlobalVariable *gvar = createGlob(Builder, "x");
	Function *fooFunc = createFunc(Builder, "foo");
	BasicBlock *entry = createBB(fooFunc, "entry");
	verifyFunction(*fooFunc);
	TheModule->print(llvm::outs(), nullptr);
	return 0;

}
