
%defines
%define parse.assert
%define api.token.constructor
%define api.value.type variant
%define parse.error detailed

%language "c++"
%require "3.5"

%code requires {
#include "runtime/runtime.hpp"
#include "ast/ast.hpp"
class Scanner;
}

%code{
#include "scanner.hpp"
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

%type <AstNode*> fn_main block_body statements statement assignment var_declaration big_expr comp_expr expr operator logic_expr xor_expr little_comp_expr arith_expr arith_add_expr arith_sub_expr boolean cell


%start fn_main

%%

// Modified rules to construct the AST nodes
fn_main
: FN_MAIN block_body {
    symtab.print();
    $$ = $2; 
    static_cast<BlockNode*>($$)->symbolTable = &symtab;  // Associate symbol table with root node
}

block_body
: DO NEWLINES statements DONE { 
    $$ = new BlockNode(); 
    for (auto& stmt : $3.get<std::vector<AstNode*>>()) {
        static_cast<BlockNode*>($$)->statements.push_back(stmt);
    }
    static_cast<BlockNode*>($$)->symbolTable = &symtab;  // Pass down symbol table to child nodes
}

statements
: statement NEWLINES { 
    $$ = std::vector<AstNode*>{buildAst($1, &symtab)}; // Pass symbol table when building AST
}
| statement NEWLINES statements { 
    $3.get<std::vector<AstNode*>>().insert($3.get<std::vector<AstNode*>>().begin(), buildAst($1, &symtab)); // Pass symbol table
    $$ = $3;
}

statement
: assignment { 
    $$ = buildAst($1, &symtab);
}
| var_declaration { 
    $$ = buildAst($1, &symtab); 
}
| operator { 
    $$ = buildAst($1, &symtab);
}

assignment
: VARIABLE ASSIGN big_expr { 
    symtab.fill_entry($1, $3->value); 
    symtab.print();
    $$ = new AssignmentNode($1, buildAst($3, &symtab));
} 

var_declaration
: VARIABLE { 
    symtab.add_entry($1); 
    std::cout << $1 << std::endl;
    $$ = new VarDeclarationNode($1); 
}

big_expr 
: comp_expr { $$ = buildAst($1, &symtab); }
| expr { $$ = buildAst($1, &symtab); }
| VARIABLE { 
    $$ = new VariableNode($1);
    $$->value = symtab.get_entry($1); // Assign value from symbol table
}

comp_expr 
: expr EQ expr { 
    $$ = new BinaryExprNode(buildAst($1, &symtab), buildAst($3, &symtab), '='); 
    $$->value = Boolean($1 == $3); 
}
| comp_expr EQ expr { 
    $$ = new BinaryExprNode(buildAst($1, &symtab), buildAst($3, &symtab), '='); 
    $$->value = Boolean($1 == $3); 
}

expr
: operator { $$ = buildAst($1, &symtab); }
| arith_expr { $$ = buildAst($1, &symtab); }
| logic_expr { $$ = buildAst($1, &symtab); }

operator
: FORWARD   { $$ = new OperatorNode("FORWARD"); }
| BACK      { $$ = new OperatorNode("BACK"); } 
| TEST      { $$ = new OperatorNode("TEST"); } 
| LOOK      { $$ = new OperatorNode("LOOK"); } 
| LEFT      { $$ = new OperatorNode("LEFT"); } 
| RIGHT     { $$ = new OperatorNode("RIGHT"); } 

logic_expr
: boolean { $$ = buildAst($1, &symtab); }
| xor_expr { $$ = buildAst($1, &symtab); }

xor_expr
: little_comp_expr { $$ = buildAst($1, &symtab); }
| xor_expr XOR little_comp_expr { 
    $$ = new BinaryExprNode(buildAst($1, &symtab), buildAst($3, &symtab), '^'); 
    $$->value = $1 ^ $3; 
}

little_comp_expr
: arith_expr LT arith_expr { 
    $$ = new BinaryExprNode(buildAst($1, &symtab), buildAst($3, &symtab), '<'); 
    $$->value = ($1 < $3); 
}
| arith_expr GT arith_expr { 
    $$ = new BinaryExprNode(buildAst($1, &symtab), buildAst($3, &symtab), '>'); 
    $$->value = ($1 > $3); 
}

arith_expr
: arith_add_expr { $$ = buildAst($1, &symtab); std::cout << $1 << std::endl;}

arith_add_expr
: arith_sub_expr { $$ = buildAst($1, &symtab); }
| arith_add_expr PLUS arith_sub_expr { 
    $$ = new BinaryExprNode(buildAst($1, &symtab), buildAst($3, &symtab), '+'); 
    $$->value = $1 + $3; 
}

arith_sub_expr
: NUMBER { $$ = new LiteralNode(Integer($1)); }
| VARIABLE { 
    $$ = new VariableNode($1);
    $$->value = symtab.get_entry($1); 
}
| arith_sub_expr MINUS NUMBER { 
    $$ = new BinaryExprNode(buildAst($1, &symtab), new LiteralNode(Integer($3)), '-'); 
    $$->value = $1 - $
3; 
}

boolean
: TRUE      { $$ = new LiteralNode(Boolean(true)); }
| FALSE     { $$ = new LiteralNode(Boolean(false)); }
| UNDEF     { $$ = new LiteralNode(Boolean(false)); }

cell
: EMPTY      { $$ = new LiteralNode(Cell($1)); }
| WALL       { $$ = new LiteralNode(Cell($1)); }
| BOX        { $$ = new LiteralNode(Cell($1)); }
| EXIT       { $$ = new LiteralNode(Cell($1)); }