bison -d decaf.y
flex decaf.l
g++ -std=c++11 lex.yy.c decaf.tab.c -lfl `llvm-config --libs core jit native --cxxflags` `llvm-config --ldflags --libs` -o decaf
clang++ -std=c++11 lex.yy.c decaf.tab.c -lfl `llvm-config --libs core jit native --cxxflags` `llvm-config --ldflags --libs` -o decaf

