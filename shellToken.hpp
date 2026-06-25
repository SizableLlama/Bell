#pragma once
#include <string>
#include <any>
#include "enums.hpp"

using namespace std;

std::string anyToString(const std::any& value) {
    if (!value.has_value()) return "nil";
    
    if (const auto* s = std::any_cast<std::string>(&value))
        return *s;
    if (const auto* d = std::any_cast<double>(&value))
        return std::to_string(*d);
    if (const auto* b = std::any_cast<bool>(&value))
        return *b ? "true" : "false";
    
    return "unknown";
}

class Token final {
public:
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;

    Token(const TokenType& type, const std::string& lexeme, 
          const std::any& literal, const int& line)
        : type(type), lexeme(lexeme), literal(literal), line(line) {}

    std::string toString() const {
        return std::to_string(static_cast<int>(type)) 
               + " " + lexeme 
               + " " + anyToString(literal);
    }
};
