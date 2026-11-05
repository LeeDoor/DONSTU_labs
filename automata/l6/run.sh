#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
cd "$SCRIPT_DIR"

g++ -std=c++17 -Wall -Wextra -pedantic \
    main.cpp Automaton.cpp AutomatonReader.cpp Renderer.cpp -o automata
exec ./automata < input.txt
