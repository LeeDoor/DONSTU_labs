#pragma once

#include <cstddef>
#include <iostream>
#include <memory>
#include <optional>
#include <string_view>
#include <utility>

template <typename T>
class LinkedStack {
public:
    explicit LinkedStack(std::size_t capacity = 0) : top_node_(nullptr) {
        static_cast<void>(capacity);
    }

    LinkedStack(const LinkedStack&) = delete;
    LinkedStack& operator=(const LinkedStack&) = delete;
    LinkedStack(LinkedStack&&) noexcept = default;
    LinkedStack& operator=(LinkedStack&&) noexcept = default;

    void push(const T& value) {
        top_node_ = std::make_unique<Node>(value, std::move(top_node_));
    }

    std::optional<T> pop() {
        if (is_empty()) {
            return std::nullopt;
        }

        T value = top_node_->value;
        top_node_ = std::move(top_node_->next);
        return value;
    }

    bool is_empty() const {
        return top_node_ == nullptr;
    }

    const void* top_address() const {
        return top_node_.get();
    }

    void print(std::ostream& out, std::string_view title) const {
        out << title << "\n";
        if (is_empty()) {
            out << "empty\n";
            return;
        }

        bool first = true;
        for (const Node* current = top_node_.get(); current != nullptr; current = current->next.get()) {
            if (!first) {
                out << ' ';
            }
            out << current->value;
            first = false;
        }
        out << "\n";
    }

private:
    struct Node {
        T value;
        std::unique_ptr<Node> next;

        Node(const T& value, std::unique_ptr<Node> next)
            : value(value), next(std::move(next)) {}
    };

    static void print_address(std::ostream& out, const void* address) {
        if (address == nullptr) {
            out << "NIL";
        } else {
            out << address;
        }
    }

    std::unique_ptr<Node> top_node_;
};
