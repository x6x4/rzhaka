#pragma once
#include <unordered_map>
#include <string>
#include <utility>
#include <variant>
#include "vars.h"


class Variable {

    int value;
    friend std::ostream& operator<<(std::ostream& os, const Variable& var) {
        os << var.value; 
        return os; 
    }
};


using ValueType = std::variant<Integer, Boolean, CellVar>;

struct Symbol_table {

    std::unordered_map<std::string, ValueType> symbol_table;

    void add_entry (const std::string &name) {
        symbol_table.insert(
            std::pair<std::string, ValueType>(name, ValueType())
        );
    }

    void fill_entry (const std::string &name, ValueType& value) {
        symbol_table[name] = value;
    }

    ValueType get_entry (const std::string &name) {
        return symbol_table[name];
    }

    void print() {

        for (const auto &entry : symbol_table) {
            std::cout << entry.first << ' ';
            std::visit([](const auto& value) {
                std::cout << "Value: " << value << std::endl;
            }, entry.second);
        }
    }
};