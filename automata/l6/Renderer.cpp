#include "Renderer.h"
#include <cstdlib>
#include <fstream>
#include <stdexcept>

// Экранирует символы, имеющие специальное значение внутри строкового литерала языка DOT.
std::string Renderer::escapeDot(const std::string& text) {
    std::string escaped;
    for (char character : text) {
        if (character == '\\' || character == '"') escaped += '\\';
        escaped += character;
    }
    return escaped;
}

// Формирует безопасный для shell аргумент, включая случай со встроенной одинарной кавычкой.
std::string Renderer::quoteShell(const std::string& text) {
    std::string quoted = "'";
    for (char character : text) {
        if (character == '\'') quoted += "'\\''";
        else quoted += character;
    }
    return quoted + "'";
}

// Описывает граф в формате DOT, визуально отличая финальные вершины двойной окружностью.
void Renderer::writeDot(std::ostream& out, const Automaton& automaton) {
    out << "digraph Automaton {\n"
        << "  rankdir=LR;\n"
        << "  node [shape=circle, fontname=\"Arial\"];\n"
        << "  edge [fontname=\"Arial\"];\n"
        << "  start [shape=point, label=\"\"];\n"
        << "  start -> q" << automaton.start << ";\n";
    for (int state = 0; state < automaton.stateCount; ++state) {
        const char* shape = automaton.finals.count(state) ? "doublecircle" : "circle";
        out << "  q" << state << " [shape=" << shape
            << ", label=\"q" << state << "\"];\n";
    }
    for (const Transition& transition : automaton.transitions) {
        out << "  q" << transition.from << " -> q" << transition.to
            << " [label=\"" << escapeDot(transition.symbol) << "\"];\n";
    }
    out << "}\n";
}

// Вызывает установленный Graphviz и превращает ошибку его запуска в явное исключение.
void Renderer::renderPng(const std::string& dotPath,
                         const std::string& imagePath) {
    const std::string command = "dot -Tpng " + quoteShell(dotPath) +
                                " -o " + quoteShell(imagePath);
    if (std::system(command.c_str()) != 0) {
        throw std::runtime_error("Graphviz failed to create " + imagePath);
    }
}

// Создает DOT-файл, проверяет запись и затем генерирует соответствующее изображение.
void Renderer::render(const Automaton& automaton, const std::string& name) {
    const std::string dotPath = name + ".dot";
    const std::string imagePath = name + ".png";
    std::ofstream dotFile(dotPath);
    if (!dotFile) throw std::runtime_error("Cannot create " + dotPath);
    writeDot(dotFile, automaton);
    dotFile.close();
    if (!dotFile) throw std::runtime_error("Cannot write " + dotPath);
    renderPng(dotPath, imagePath);
}
