#pragma once // this makes sure to include this header file only once
#include <string>
#include <vector>
#include <iostream>
#include <optional>

enum class TokenType
{
    exit,
    int_literal,
    semicolon
};
std::ostream &operator<<(std::ostream &os, TokenType s)
{
    switch (s)
    {
    case TokenType::exit:
        os << "exit";
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

class Tokenizer
{
public:
    // Just taking a std::string, because it works well when given an lvalue or a string literal.
    // If takes std::string&, it will leave the caller in unspecified state (cuz we steal the buffer from the caller)
    explicit Tokenizer(std::string src) : src_(std::move(src)), index_{0} {}

    auto tokenize() -> std::vector<Token>
    {
        auto tokens = std::vector<Token>{};
        auto buffer = std::string{};

        while (peek().has_value())
        {
            char c = consume();
            if (std::isalpha(c))
            {
                // got an alpha character, it must be the start of a non-int token
                // now keep collecting the rest of the token
                buffer.push_back(c);

                while (peek().has_value() && std::isalnum(peek().value()))
                {
                    buffer.push_back(consume());
                }

                if (buffer == "exit")
                {
                    tokens.push_back(Token{.type = TokenType::exit, .value = std::nullopt});
                    buffer.clear();
                }
                else
                {
                    std::cerr << "Invalid token: " << c << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if (std::isdigit(c))
            {
                buffer.push_back(c);

                while (peek().has_value() && std::isdigit(peek().value()))
                {
                    buffer.push_back(consume());
                }

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

private:
    /**
     * @brief The source code to be tokenized.
     */
    const std::string src_;

    /**
     * @brief The index that we are currently up to within the source code string.
     * @example If `index_ == 2`, it means that we have consumed index 0 and 1, but not index 2 yet.
     */
    size_t index_;

    /**
     * @brief Peek the character in the source code, which is `offset` characters from the current index.
     *
     * @param offset The offset from current index.
     * @return If in bounds, returns the character that is `offset` characters from the current index. Otherwise, returns `std::nullopt`.
     */
    [[nodiscard]] auto peek(size_t offset = 0) const -> std::optional<char>
    {
        if (index_ + offset >= src_.size())
        {
            return std::nullopt;
        }
        return src_.at(index_ + offset);
    };

    /**
     * @brief Consumes the next character in the source code.
     *
     * @return The next character in the source code.
     */
    auto consume() -> char
    {
        return src_.at(index_++);
    }
};