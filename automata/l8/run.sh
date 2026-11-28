#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"
g++ -std=c++17 -Wall -Wextra -O2 -o lab main.cpp Grammar.cpp GrammarBuilder.cpp Automaton.cpp Renderer.cpp
./lab $@
