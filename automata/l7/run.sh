#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

input="${1:-grammar.dat}"

g++ -std=c++17 -Wall -Wextra -O2 -o grammar_type main.cpp Grammar.cpp GrammarBuilder.cpp
./grammar_type < "$input"
