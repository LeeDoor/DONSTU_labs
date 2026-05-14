#pragma once

#include <iostream>
#include <memory>
#include <string_view>

class LinkedList {
public:
    void push_back(int value) {
        auto new_node = std::make_unique<Node>(value);
        if (head_ == nullptr) {
            head_ = std::move(new_node);
            return;
        }

        Node* current = head_.get();
        while (current->next != nullptr) {
            current = current->next.get();
        }
        current->next = std::move(new_node);
    }

    bool is_empty() const {
        return head_ == nullptr;
    }

    void move_max_to_front() {
        if (head_ == nullptr || head_->next == nullptr) {
            return;
        }

        std::unique_ptr<Node>* max_link = &head_;
        for (std::unique_ptr<Node>* link = &head_; (*link)->next != nullptr; link = &((*link)->next)) {
            if ((*link)->next->value > (*max_link)->value) {
                max_link = &((*link)->next);
            }
        }

        if (max_link == &head_) {
            return;
        }

        std::unique_ptr<Node> max_node = std::move(*max_link);
        *max_link = std::move(max_node->next);
        max_node->next = std::move(head_);
        head_ = std::move(max_node);
    }

    void print(std::ostream& out, std::string_view title) const {
        out << title << "\n";
        if (is_empty()) {
            out << "empty\n";
            return;
        }

        bool first = true;
        for (const Node* current = head_.get(); current != nullptr; current = current->next.get()) {
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
        int value;
        std::unique_ptr<Node> next;

        explicit Node(int value) : value(value), next(nullptr) {}
    };

    static void print_address(std::ostream& out, const void* address) {
        if (address == nullptr) {
            out << "NIL";
        } else {
            out << address;
        }
    }

    std::unique_ptr<Node> head_;
};
