#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include "tokenization.hpp"

auto tokens_to_asm(const std::vector<Token> &tokens) -> std::string
{
    std::stringstream output;
    output << "global _start\n_start:\n";

    for (int i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i].type == TokenType::exit)
        {
            // only proceeds if next 2 tokens are: int_literal then semicolon
            if (i + 1 >= tokens.size() || tokens[i + 1].type != TokenType::int_literal)
                continue;

            if (i + 2 >= tokens.size() || tokens[i + 2].type != TokenType::semicolon)
                continue;

            std::string int_literal = tokens[i + 1].value.value();
            output << "\tmov rax, 60\n";
            output << "\tmov rdi, " << int_literal << "\n";
            output << "\tsyscall";
            i += 2;
        }
    }
    return output.str();
}
auto main(int argc, char *argv[]) -> int
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./hydro <input.hy>" << std::endl;
        return EXIT_FAILURE;
    }

    auto file_path = argv[1];
    auto contents = std::string{};
    // putting all these into a scope so that input stream will automatically closes
    {
        auto input = std::fstream{file_path, std::ios_base::in};
        auto contents_stream = std::stringstream{};
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    auto tokenizer = Tokenizer{contents};
    auto tokens = tokenizer.tokenize();
    {
        auto output = std::ofstream{"../hydro_example.asm", std::ios_base::out};
        output << tokens_to_asm(tokens);
    }

    system("nasm -felf64 ../hydro_example.asm");
    system("ld -o out ../hydro_example.o");

    return EXIT_SUCCESS;
}