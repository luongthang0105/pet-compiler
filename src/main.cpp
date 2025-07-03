#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>

enum class TokenType
{
    _return,
    int_literal,
    semicolon
};

struct Token
{
    TokenType type;
    std::optional<std::string> value;
};

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

    std::cout << contents << std::endl;
    return EXIT_SUCCESS;
}