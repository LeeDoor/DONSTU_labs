#pragma once
#include "Automaton.h"
#include <string>

void renderNFA(const Automaton& a, const std::string& name);
void renderDFA(const Automaton& dfa, const std::string& name);
