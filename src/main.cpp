#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

enum class TokenType
{
    _return,
    int_literal,
    semicolon
};
std::ostream &operator<<(std::ostream &os, TokenType s)
{
    switch (s)
    {
    case TokenType::_return:
        os << "_return";
        break;
    case TokenType::int_literal:
        os << "int_literal";
        break;
    case TokenType::semicolon:
        os << "semicolon";
        break;
    default:
        os << "Unknown Token";
        break;
    }
    return os;
}
struct Token
{
    TokenType type;
    std::optional<std::string> value;
};

std::ostream &operator<<(std::ostream &output, const Token &token)
{
    output << "Token: {type = " << token.type;
    if (token.value.has_value())
    {
        output << ", value = " << token.value.value() << "}";
    }
    else
    {
        output << "}";
    }
    return output;
}
auto tokenize(const std::string &str) -> std::vector<Token>
{
    auto tokens = std::vector<Token>{};
    auto buffer = std::string{};

    for (int i = 0; i < str.size(); ++i)
    {
        char c = str[i];
        if (std::isalpha(c))
        {
            // got an alpha character, it must be the start of a non-int token
            // now keep collecting the rest of the token
            buffer.push_back(c);

            ++i;
            while (i < str.size() && std::isalnum(str[i]))
            {
                buffer.push_back(str[i]);
                ++i;
            }
            --i;

            if (buffer == "return")
            {
                tokens.push_back(Token{.type = TokenType::_return, .value = std::nullopt});
                buffer.clear();
            }
        }
        else if (std::isdigit(c))
        {
            buffer.push_back(c);

            ++i;
            while (i < str.size() && std::isdigit(str[i]))
            {
                buffer.push_back(str[i]);
                ++i;
            }
            --i;

            tokens.push_back(Token{.type = TokenType::int_literal, .value = buffer});
            buffer.clear();
        }
        else if (c == ';')
        {
            tokens.push_back(Token{.type = TokenType::semicolon, .value = std::nullopt});
        }
        else if (std::isspace(c))
        {
            continue;
        }
        else
        {
            std::cerr << "Invalid token: " << c << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return tokens;
}

auto tokens_to_asm(const std::vector<Token> &tokens) -> std::string
{
    std::stringstream output;
    output << "global _start\n_start:\n";

    for (int i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i].type == TokenType::_return)
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

    auto tokens = tokenize(contents);
    {
        auto output = std::ofstream{"../hydro_example.asm", std::ios_base::out};
        output << tokens_to_asm(tokens);
    }
    return EXIT_SUCCESS;
}