#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <unordered_map>

#include "../liba.hpp"



// Define a base AST node class
class AstNode {
public:
    ValueType value; 
    Symbol_table* symbolTable; // Pointer to the symbol table

    AstNode() : value(ValueType()), symbolTable(nullptr) {}

    virtual ~AstNode() {}

    virtual ValueType interpret() = 0;
};

// AST node for a variable declaration
class VarDeclarationNode : public AstNode {
public:
    std::string variable;

    VarDeclarationNode(const std::string& variable) : variable(variable) {}

    ValueType interpret() override {
        if (symbolTable) {
            symbolTable->add_entry(variable); // Add to symbol table
        }
        return ValueType();
    }
};

// AST node for an assignment
class AssignmentNode : public AstNode {
public:
    std::string variable;
    AstNode* expression;

    AssignmentNode(const std::string& variable, AstNode* expression) : 
        variable(variable), expression(expression) {}

    ValueType interpret() override {
        if (symbolTable) {
            symbolTable->fill_entry(variable, expression->interpret()); 
        }
        return ValueType();
    }
};

// AST node for a binary expression
class BinaryExprNode : public AstNode {
public:
    AstNode* left;
    AstNode* right;
    char op;

    BinaryExprNode(AstNode* left, AstNode* right, char op) :
        left(left), right(right), op(op) {}

    ValueType interpret() override {
        ValueType leftValue = left->interpret();
        ValueType rightValue = right->interpret();

        if (op == '+') {
            if (std::holds_alternative<Integer>(leftValue) && std::holds_alternative<Integer>(rightValue)) {
                value = std::get<Integer>(leftValue) + std::get<Integer>(rightValue);
            } else {
                // Handle other types (e.g., string concatenation, etc.)
            }
        } else if (op == '-') {
            // ... (Implement other binary operators)
        } else if (op == '^') {
            if (std::holds_alternative<Boolean>(leftValue) && std::holds_alternative<Boolean>(rightValue)) {
                value = std::get<Boolean>(leftValue) ^ std::get<Boolean>(rightValue);
            } else {
                // Handle other types
            }
        } else if (op == '=') {            if (std::holds_alternative<Integer>(leftValue) && std::holds_alternative<Integer>(rightValue)) {
                value = Boolean(std::get<Integer>(leftValue) == std::get<Integer>(rightValue));
            } else if (std::holds_alternative<Boolean>(leftValue) && std::holds_alternative<Boolean>(rightValue)) {
                value = Boolean(std::get<Boolean>(leftValue) == std::get<Boolean>(rightValue));
            } else {
                // Handle other types
            }
        } else if (op == '<') {
            if (std::holds_alternative<Integer>(leftValue) && std::holds_alternative<Integer>(rightValue)) {
                value = Boolean(std::get<Integer>(leftValue) < std::get<Integer>(rightValue));
            } else {
                // Handle other types
            }
        } else if (op == '>') {
            if (std::holds_alternative<Integer>(leftValue) && std::holds_alternative<Integer>(rightValue)) {
                value = Boolean(std::get<Integer>(leftValue) > std::get<Integer>(rightValue));
            } else {
                // Handle other types
            }
        }
        return value;
    }
};

// AST node for a unary expression
class UnaryExprNode : public AstNode {
public:
    AstNode* expr;
    char op;

    UnaryExprNode(AstNode* expr, char op) :
        expr(expr), op(op) {}

    ValueType interpret() override {
        // ... (Implement unary operators)
        return ValueType();
    }
};

// AST node for a variable reference
class VariableNode : public AstNode {
public:
    std::string variable;

    VariableNode(const std::string& variable) : variable(variable) {}

    ValueType interpret() override {
        if (symbolTable) {
            value = symbolTable->get_entry(variable); 
        }
        return value;
    }
};

// AST node for a literal value
class LiteralNode : public AstNode {
public:
    ValueType value;

    LiteralNode(ValueType value) : value(value) {}

    ValueType interpret() override {
        return value;
    }
};

// AST node for an operator (e.g., FORWARD, BACK, etc.)
class OperatorNode : public AstNode {
public:
    std::string op;

    OperatorNode(const std::string& op) : op(op) {}

    ValueType interpret() override {
        if (op == "FORWARD") {
            // ... (Call your Lab function for FORWARD)
        } else if (op == "BACK") {
            // ... (Call your Lab function for BACK)
        } else if (op == "TEST") {
            // ... (Call your Lab function for TEST)
        } else if (op == "LOOK") {
            // ... (Call your Lab function for LOOK)
        } else if (op == "LEFT") {
            // ... (Call your Lab function for LEFT)
        } else if (op == "RIGHT") {
            // ... (Call your Lab function for RIGHT)
        }
        return ValueType();
    }
};

// AST node for a block of statements
class BlockNode : public AstNode {
public:
    std::vector<AstNode*> statements;

    BlockNode() {}

    ValueType interpret() override {
        for (auto& stmt : statements) {
            stmt->interpret(); // Execute each statement
        }
        return ValueType();
    }
};


