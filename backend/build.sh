rm -f parser.tab.cc parser.tab.hh lex.yy.cc

bison -d parser.y
flex scanner.l
g++ -Wno-register -o a.out runtime/runtime.cpp lex.yy.cc parser.tab.cc parser.cc  
