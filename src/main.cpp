#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include "tokenization.hpp"
#include "parser.hpp"
#include "generation.hpp"

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

    auto tokenizer = Tokenizer{std::move(contents)};
    auto tokens = tokenizer.tokenize();

    auto parser = Parser{std::move(tokens)};
    auto tree = parser.parse();

    if (!tree.has_value())
    {
        std::cerr << "No VALID exit statement found" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto generation = Generation(std::move(tree.value()));
    {
        auto output = std::ofstream{"../hydro_example.asm", std::ios_base::out};
        output << generation.generate();
    }

    system("nasm -felf64 ../hydro_example.asm");
    system("ld -o out ../hydro_example.o");

    return EXIT_SUCCESS;
}