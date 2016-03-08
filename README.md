# Compiler
For decaf programming language

This is a toy compiler for decaf programming language.
It uses flex for lexical analysis and bison for parsing.
decaf.l is the flex file
decaf.y is the bison file
ast.h makes an abstract syntax tree
Visitor.h is the traversing the ast
XMLVisitor.h makes an XML structure and outputs the same
CodeGenVisitor.h makes an LLVM IR


To run : Make hmmm.sh an exucatable file : chmod +x hmmm.sh
And then simply execute it.
./hmmm.sh
