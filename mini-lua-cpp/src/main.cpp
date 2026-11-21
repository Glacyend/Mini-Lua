#include "errors.h"
#include "location.h"
#include "scanner.h"
#include "token.h"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <print>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

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
    std::string source_string = std::move(source);

    Scanner scanner(source_string);

    try {
        scanner.scan_tokens();
    } catch (const LexicalError& error) {
        std::println(std::cerr, "{}", error.what());
        std::exit(1);
    }

    auto [tokens, line_offset] = scanner.get_result();
    for (const Token& token : tokens) {
        auto get_string_view = [&source_string, &line_offset](Location location) {
            auto line_start = source_string.begin()
                            + static_cast<std::ptrdiff_t>(line_offset[location.start.line]);
            auto start = line_start + static_cast<std::ptrdiff_t>(location.start.column);
            auto end = line_start + static_cast<std::ptrdiff_t>(location.end.column);
            return std::string_view(start, end);
        };

        std::visit([&get_string_view]<typename T>(const T& value) {
            if constexpr (std::is_same_v<T, TokenReserved>) {
                std::println("[RESERVED] {}", get_string_view(value.location));
            } else if constexpr (std::is_same_v<T, TokenNumber>) {
                std::println("[NUMBER] {}", get_string_view(value.location));
            } else if constexpr (std::is_same_v<T, TokenString>) {
                std::println("[STRING] {}", get_string_view(value.location));
            } else if constexpr (std::is_same_v<T, TokenSymbol>) {
                std::println("[SYMBOL] {}", get_string_view(value.location));
            } else if constexpr (std::is_same_v<T, TokenName>) {
                std::println("[NAME] {}", get_string_view(value.location));
            } else if constexpr (std::is_same_v<T, TokenEOL>) {
                std::println("[EOL]");
            }
        }, token);
    }
}
