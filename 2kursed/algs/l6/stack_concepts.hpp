#pragma once

#include <concepts>
#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string_view>

template <typename Stack, typename Value>
concept StackType = std::constructible_from<Stack, std::size_t> &&
    requires(Stack stack, Value value, std::ostream& out, std::string_view title) {
        { stack.push(value) } -> std::same_as<void>;
        { stack.pop() } -> std::same_as<std::optional<Value>>;
        { stack.is_empty() } -> std::same_as<bool>;
        { stack.top_address() } -> std::same_as<const void*>;
        { stack.print(out, title) } -> std::same_as<void>;
    };
