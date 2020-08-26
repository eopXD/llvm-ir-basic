#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

static Module *TheModule;
static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);

int main ()
{
	llvm::LLVMContext &Context = TheContext;
	TheModule = new Module("my compiler", Context);
	TheModule->print(llvm::outs(), nullptr);
	return 0;
}
