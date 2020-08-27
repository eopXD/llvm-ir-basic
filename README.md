## How to use

Go through the commit to learn LLVM IR.

## LLVM IR syntax basic

Using LLVM IR C++ API to generate LLVM IR.

Covered:

- Create module
- Create function
- Create Basic Block
- Emit global variable
- Declare If-else function
- Declare Loop
- Pointer manipulation
- Vector manipulation

## Environment

```
$ lli --version
LLVM (http://llvm.org/):
  LLVM version 10.0.1
  Optimized build.
  Default target: x86_64-apple-darwin18.2.0
  Host CPU: broadwell
$ clang++ --version
clang version 10.0.1
Target: x86_64-apple-darwin18.2.0
Thread model: posix
InstalledDir: /usr/local/opt/llvm/bin
```

Compile and run (remove `libxml2` from compile option if not found):

```
$ clang++ module.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core` -fno-rtti -o module
$ ./module
```

## Tips

- `ICmp` instruciton output variable of type `Int1`
- `getelementptr` calculates address, doesn't access data
- Vector types are used when multiple primitive data are operated in parallel
- Phi Node is used to implement the φ node in the SSA graph representing the function. It is an instruction used to select a value depending on the predecessor of the current block. Phi nodes are necessary due to the structure of the SSA (static single assignment) style of the LLVM code.
