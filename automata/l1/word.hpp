#pragma once
#include "letter.hpp"
#include <vector>

using Word = std::vector<Letter>;

std::optional<Word> word_from_stream(std::istream& is);
std::ostream& operator<<(std::ostream& os, const Word& word);
