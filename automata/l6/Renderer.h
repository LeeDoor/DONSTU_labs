#pragma once

#include "Automaton.h"
#include <iosfwd>
#include <string>

class Renderer {
public:
    // Записывает описание автомата в DOT и просит Graphviz создать PNG с указанным базовым именем.
    static void render(const Automaton& automaton, const std::string& name);

private:
    // Экранирует обратные слеши и кавычки, чтобы символ перехода оставался корректной строкой DOT.
    static std::string escapeDot(const std::string& text);
    // Заключает аргумент команды в одинарные кавычки, корректно экранируя содержащиеся в нем кавычки.
    static std::string quoteShell(const std::string& text);
    // Записывает узлы, начальную стрелку, финальные состояния и дуги автомата в DOT-поток.
    static void writeDot(std::ostream& out, const Automaton& automaton);
    // Запускает Graphviz для преобразования DOT-файла в PNG и сообщает о неудачном завершении.
    static void renderPng(const std::string& dotPath,
                          const std::string& imagePath);
};
