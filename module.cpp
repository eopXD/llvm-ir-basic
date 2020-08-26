#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Verifier.h"

#include <vector>
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
int main ( int argc, char *argv[] )
{
	llvm::LLVMContext &Context = TheContext;
	TheModule = new Module("my compiler", Context);
	Function *fooFunc = createFunc(Builder, "foo");

	verifyFunction(*fooFunc);
	TheModule->print(llvm::outs(), nullptr);
	return 0;

}
