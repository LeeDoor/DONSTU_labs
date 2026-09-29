#pragma once
#include <optional>
#include <ostream>

class Letter {
public:
    using Value = char;
    static std::optional<Letter> char_to_letter(char c);

    bool operator==(const Letter& other) const;
    bool operator<(const Letter& other) const;

    Value get_value() const;

private:
    static bool is_letter(char c);
    explicit Letter(Value letter);

    Value value;
};

 std::ostream& operator<<(std::ostream& os, const Letter& letter);
