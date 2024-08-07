
%defines
%define parse.assert
%define api.token.constructor
%define api.value.type variant
%define parse.error detailed

%language "c++"
%require "3.5"

%code requires {
#include "liba.h"
#include "runtime/runtime.h"
class Scanner;
}

%code{
#include "scanner.hh"
#include <iostream>
#include <fstream>


extern "C" int yyerror(const char *s) { 
    std::cout << s << std::endl;
    return 0;
};

Symbol_table symtab;
Field lab("/home/cracky/rzhaka/backend/hexagons.labhex");


static yy::parser::symbol_type yylex(Scanner &scanner) {
    return scanner.ScanToken();
}

}

%lex-param { Scanner &scanner }
%parse-param { Scanner &scanner }


%token <char> PLUS MINUS XOR EQ LT GT
%token <Integer> NUMBER
%token <Boolean> TRUE FALSE UNDEF 
%token <CellVar> EMPTY WALL BOX EXIT 
%token <std::string> VARIABLE ASSIGN
%token FN_MAIN DO DONE NEWLINES 
%token FORWARD BACK LEFT RIGHT TEST LOOK
%token IF

%type <ValueType> expr
%type <Integer>   arith_expr arith_sub_expr arith_add_expr 
%type <ValueType> assignment operator big_expr comp_expr
%type <Boolean>   boolean logic_expr xor_expr little_comp_expr
%type <Cell>      cell

%start fn_main

%%

fn_main 
: FN_MAIN block_body { symtab.print(); }


block_body
: DO NEWLINES statements DONE 

statements
: statement NEWLINES
| statement NEWLINES statements 

statement
: assignment 
| var_declaration
| operator

assignment
: VARIABLE ASSIGN big_expr { 
    symtab.fill_entry($1, $3); symtab.print();
} 

var_declaration: 
VARIABLE { symtab.add_entry($1); std::cout << $1 << std::endl; }

big_expr 
: comp_expr { $$ = $1; }
| expr { $$ = $1; }
| VARIABLE  { $$ = symtab.get_entry($1);}

comp_expr 
: expr EQ expr { 
    $$ = Boolean($1 == $3); 
}
| comp_expr EQ expr { 
    $$ = Boolean($1 == $3); 
}

expr
: operator
| arith_expr { $$ = $1; }
| logic_expr { $$ = $1; }

operator
: FORWARD   { $$ = lab.move_robot(1); }
| BACK      { $$ = lab.move_robot(0); } 
| TEST      { $$ = lab.test(); } 
| LOOK      { $$ = lab.look(); } 
| LEFT      { $$ = lab.left(); } 
| RIGHT     { $$ = lab.right(); } 

logic_expr
: boolean { $$ = $1; }
| xor_expr { $$ = $1; }

xor_expr
: little_comp_expr { $$ = $1; }
| xor_expr XOR little_comp_expr { $$ = $1 ^ $3; }

little_comp_expr
: arith_expr LT arith_expr { $$ = ($1 < $3); }
| arith_expr GT arith_expr { $$ = ($1 > $3); }


arith_expr
: arith_add_expr { $$ = $1; std::cout << $1 << std::endl;}


arith_add_expr
: arith_sub_expr { $$ = $1; }
| arith_add_expr PLUS arith_sub_expr { $$ = $1 + $3; }


arith_sub_expr
: NUMBER { $$ = $1; }
| VARIABLE { $$ = symtab.get_entry($1);}
| arith_sub_expr MINUS NUMBER { $$ = $1 - $3; }


boolean
: TRUE      { $$ = $1; }
| FALSE     { $$ = $1; }
| UNDEF     { $$ = $1; }

cell
: EMPTY      { $$ = $1; }
| WALL       { $$ = $1; }
| BOX        { $$ = $1; }
| EXIT       { $$ = $1; }


%%

void
yy::parser::error(const std::string& m)
{
  std::cerr << m << std::endl;
}