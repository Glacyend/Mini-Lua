#include "errors.h"
#include "expr.h"
#include "parser.h"
#include "scanner.h"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <print>
#include <string>
#include <utility>

static constexpr std::size_t BUFFER_SIZE = 64 * 1024;

static char read_buffer[BUFFER_SIZE];

void process(std::string source);

int main() {
    std::string content;

    while (std::cin.read(read_buffer, BUFFER_SIZE)) {
        content.append(read_buffer, static_cast<std::size_t>(std::cin.gcount()));
    }
    if (std::cin.gcount() > 0) {
        content.append(read_buffer, static_cast<std::size_t>(std::cin.gcount()));
    }

    process(std::move(content));

    return 0;
}

void process(std::string source) {
    Scanner scanner(source);

    try {
        scanner.scan_tokens();
    } catch (const LexicalError& error) {
        std::println(std::cerr, "{}", error.what());
        std::exit(1);
    }

    auto tokens = scanner.get_tokens();

#ifdef DEBUG_MODE
    std::println("===== TOKENS =====");
    for (const Token& token : tokens) {
        std::println("{}", token);
    }
    std::println("==================");
#endif

    Parser parser(std::move(tokens));

    std::vector<std::unique_ptr<Expr>> expressions;
    try {
        expressions = parser.parse_expressions();
    } catch (const SyntaxError& error) {
        std::println(std::cerr, "{}", error.what());
        std::exit(1);
    }

#ifdef DEBUG_MODE
    std::println("===== EXPR =====");
    for (const auto& expr : expressions) {
        std::println("{}", *expr);
    }
    std::println("================");
#endif
}
