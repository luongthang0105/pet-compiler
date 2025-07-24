#pragma once

#include "parser.hpp"
#include <sstream>

class Generation
{
public:
    Generation(NodeExit exit) : exit_{exit} {};

    [[nodiscard]] auto generate() const -> std::string
    {
        std::stringstream output;
        output << "global _start\n_start:\n";

        std::string int_literal = exit_.nodeExpr.int_lit.value.value();
        output << "\tmov rax, 60\n";
        output << "\tmov rdi, " << int_literal << "\n";
        output << "\tsyscall";
        return output.str();
    }

private:
    NodeExit exit_;
};