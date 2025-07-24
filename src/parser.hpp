#pragma once
#include <vector>
#include "tokenization.hpp"

struct NodeExpr
{
    Token int_lit;
};

struct NodeExit
{
    NodeExpr nodeExpr;
};

class Parser
{
public:
    explicit Parser(std::vector<Token> tokens) : tokens_{std::move(tokens)}, index_{0} {};

    auto parse_expr() -> std::optional<NodeExpr>
    {
        if (peek().has_value() && peek().value().type == TokenType::int_literal)
        {
            return NodeExpr{.int_lit = consume()};
        }
        return std::nullopt;
    }
    auto parse() -> std::optional<NodeExit>
    {
        std::optional<NodeExit> exit_node;

        while (peek().has_value())
        {
            Token token = consume();
            if (token.type == TokenType::exit)
            {
                if (auto expr_node = parse_expr())
                {
                    exit_node = NodeExit{.nodeExpr = expr_node.value()};
                }
                else
                {
                    std::cerr << "Invalid expression" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
        return exit_node;
    }

private:
    /**
     * @brief The token list received from the `Tokenizer`.
     */
    std::vector<Token> tokens_;

    /**
     * @brief The index that we are currently up to within `tokens_`.
     * @example If `index_ == 2`, it means that we have consumed index 0 and 1, but not index 2 yet.
     */
    size_t index_;

    /**
     * @brief Peek a token in `tokens_`, which is `offset` tokens from the current index.
     *
     * @param offset The offset from current index.
     * @return If in bounds, returns a token that is `offset` tokens from the current index. Otherwise, returns `std::nullopt`.
     */
    [[nodiscard]] auto peek(size_t offset = 0) const -> std::optional<Token>
    {
        if (index_ + offset >= tokens_.size())
        {
            return std::nullopt;
        }
        return tokens_.at(index_ + offset);
    };

    /**
     * @brief Consumes the token at current index in `tokens_`, then increment current index by one.
     *
     * @return The token at current index in `tokens_`.
     */
    auto consume() -> Token
    {
        return tokens_.at(index_++);
    }
};