#include "Automaton.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <ostream>
#include <queue>
#include <stdexcept>

struct Automaton::MinimizationContext {
    int sink = 0; // Искусственный тупиковый индекс для неописанных переходов.
    int totalStates = 0; // Число исходных состояний вместе с искусственным sink.
    bool omitSink = false; // Нужно ли опустить отдельный rejecting sink в итоговом автомате.
    std::set<std::string> alphabet; // Полный алфавит, по которому уточняются классы и строятся переходы.
    std::vector<std::map<std::string, int>> transitionTable; // Быстрый поиск цели по исходному состоянию и символу.
    std::vector<int> states; // Достижимые исходные индексы; недостижимые состояния не минимизируются.
    std::vector<int> blockOf; // Текущий номер класса эквивалентности для каждого индекса.
    std::vector<std::vector<int>> equivalences; // Снимки всех разбиений для показа шагов алгоритма.
    std::vector<std::vector<int>> blocks; // Списки состояний каждого класса окончательного разбиения.
    std::vector<int> number; // Соответствие класса его новому номеру в выходном автомате.
    std::vector<int> order; // Порядок обхода классов от начального для стабильной нумерации результата.
};

// Сериализует автомат в текстовый формат, который использует программа для входных данных.
void Automaton::print(std::ostream& out) const {
    out << stateCount << '\n';
    bool first = true;
    for (int state : finals) {
        if (!first) out << ' ';
        out << state;
        first = false;
    }
    out << '\n';
    for (const Transition& transition : transitions) {
        out << transition.from << ' ' << transition.to << ' '
            << transition.symbol << '\n';
    }
}

// Отклоняет некорректное описание автомата до запуска алгоритма минимизации.
void Automaton::validate() const {
    if (stateCount <= 0 || start < 0 || start >= stateCount) {
        throw std::runtime_error("Invalid automaton state count or start state.");
    }
    for (int state : finals) {
        if (state < 0 || state >= stateCount) {
            throw std::runtime_error("A final state is outside the automaton.");
        }
    }
}

// Инициализирует общие рабочие массивы и резервирует индекс состояния sink.
Automaton::MinimizationContext Automaton::createContext() const {
    MinimizationContext context;
    context.sink = stateCount;
    context.totalStates = stateCount + 1;
    context.alphabet = alphabet;
    context.blockOf.assign(context.totalStates, -1);
    return context;
}

// Преобразует список переходов в таблицу и проверяет, что из пары (состояние, символ) есть не более одной дуги.
void Automaton::buildTransitionTable(MinimizationContext& context) const {
    context.transitionTable.resize(stateCount);
    for (const Transition& transition : transitions) {
        if (transition.from < 0 || transition.from >= stateCount ||
            transition.to < 0 || transition.to >= stateCount) {
            throw std::runtime_error("A transition state is outside the automaton.");
        }
        if (!context.transitionTable[transition.from]
                 .emplace(transition.symbol, transition.to).second) {
            throw std::runtime_error("The automaton is not deterministic.");
        }
        context.alphabet.insert(transition.symbol);
    }
}

// Возвращает заданную дугой цель, а для пропуска в таблице или уже достигнутого sink — sink.
int Automaton::nextState(const MinimizationContext& context, int state,
                         const std::string& symbol) const {
    if (state == context.sink) return context.sink;
    const auto transition = context.transitionTable[state].find(symbol);
    if (transition == context.transitionTable[state].end()) return context.sink;
    return transition->second;
}

// Выполняет обход в ширину от начального состояния по всему алфавиту и исключает недостижимые вершины.
void Automaton::findReachableStates(MinimizationContext& context) const {
    std::vector<bool> reachable(context.totalStates, false);
    std::queue<int> queue;
    reachable[start] = true;
    queue.push(start);
    while (!queue.empty()) {
        const int state = queue.front();
        queue.pop();
        for (const std::string& symbol : context.alphabet) {
            const int target = nextState(context, state, symbol);
            if (!reachable[target]) {
                reachable[target] = true;
                queue.push(target);
            }
        }
    }
    for (int state = 0; state < context.totalStates; ++state) {
        if (reachable[state]) context.states.push_back(state);
    }
}

// Создает 0-эквивалентность: финальные состояния отделяются от нефинальных.
void Automaton::initializePartition(MinimizationContext& context) const {
    std::map<bool, int> blockIds;
    for (int state : context.states) {
        const bool accepting = state != context.sink && finals.count(state);
        const auto inserted = blockIds.emplace(accepting, blockIds.size());
        context.blockOf[state] = inserted.first->second;
    }
    context.equivalences.push_back(context.blockOf);
}

// Проверяет равенство разбиений сравнением того, какие пары состояний находятся в одном классе.
bool Automaton::samePartition(const std::vector<int>& first,
                             const std::vector<int>& second,
                             const std::vector<int>& states) const {
    for (size_t i = 0; i < states.size(); ++i) {
        for (size_t j = i + 1; j < states.size(); ++j) {
            if ((first[states[i]] == first[states[j]]) !=
                (second[states[i]] == second[states[j]])) return false;
        }
    }
    return true;
}

// Строит новые классы по старому классу состояния и классам его целей на всех символах, пока разбиение не стабилизируется.
void Automaton::refinePartitions(MinimizationContext& context) const {
    while (true) {
        std::map<std::vector<int>, int> blockIds;
        std::vector<int> refined(context.totalStates, -1);
        for (int state : context.states) {
            std::vector<int> signature = {context.blockOf[state]};
            for (const std::string& symbol : context.alphabet) {
                signature.push_back(
                    context.blockOf[nextState(context, state, symbol)]);
            }
            const auto inserted = blockIds.emplace(signature, blockIds.size());
            refined[state] = inserted.first->second;
        }
        context.equivalences.push_back(refined);
        const bool stable =
            samePartition(context.blockOf, refined, context.states);
        context.blockOf.swap(refined);
        if (stable) return;
    }
}

// Печатает состояние для трассировки: начальное зеленым, финальное красным, искусственный sink текстовой меткой.
void Automaton::printState(const MinimizationContext& context, int state) const {
    const bool accepting = state != context.sink && finals.count(state);
    const char* color = accepting ? "\033[31m" : "\033[0m";
    if (!accepting && state == start) color = "\033[32m";
    std::cout << color;
    if (state == context.sink) std::cout << "sink";
    else std::cout << 'q' << state;
    std::cout << "\033[0m";
}

// Отображает указанное разбиение в виде множества классов и вызывает цветной вывод для каждого состояния.
void Automaton::printPartition(const MinimizationContext& context,
                               size_t order) const {
    std::map<int, std::vector<int>> blocks;
    for (int state : context.states) {
        blocks[context.equivalences[order][state]].push_back(state);
    }
    std::cout << order << "-эквивалентность: ";
    bool firstBlock = true;
    for (const auto& block : blocks) {
        if (!firstBlock) std::cout << ' ';
        std::cout << '{';
        for (size_t i = 0; i < block.second.size(); ++i) {
            if (i) std::cout << ", ";
            printState(context, block.second[i]);
        }
        std::cout << '}';
        firstBlock = false;
    }
    std::cout << '\n';
}

// Последовательно выводит историю уточнений и подтверждает равенство двух последних эквивалентностей.
void Automaton::printPartitionHistory(const MinimizationContext& context) const {
    for (size_t i = 0; i < context.equivalences.size(); ++i) {
        printPartition(context, i);
    }
    const size_t last = context.equivalences.size() - 1;
    const bool equal = samePartition(context.equivalences[last - 1],
                                     context.equivalences[last], context.states);
    std::cout << "E_" << last - 1 << " = E_" << last << ": "
              << (equal ? "да" : "нет") << '\n';
}

// Преобразует массив номеров классов в списки их состояний для дальнейшей сборки автомата.
void Automaton::buildBlocks(MinimizationContext& context) const {
    int blockCount = 0;
    for (int state : context.states) {
        blockCount = std::max(blockCount, context.blockOf[state] + 1);
    }
    context.blocks.resize(blockCount);
    for (int state : context.states) {
        context.blocks[context.blockOf[state]].push_back(state);
    }
}

// Присваивает классам итоговые номера в порядке обхода от начального класса и решает, исключать ли sink.
void Automaton::numberBlocks(MinimizationContext& context) const {
    const int startBlock = context.blockOf[start];
    const int sinkBlock = context.blockOf[context.sink];
    context.omitSink = sinkBlock >= 0 && sinkBlock != startBlock;
    context.number.assign(context.blocks.size(), -1);
    context.order.push_back(startBlock);
    context.number[startBlock] = 0;
    std::queue<int> queue;
    queue.push(startBlock);
    while (!queue.empty()) {
        const int block = queue.front();
        queue.pop();
        const int representative = context.blocks[block].front();
        for (const std::string& symbol : context.alphabet) {
            const int target = context.blockOf[nextState(context, representative, symbol)];
            if (context.omitSink && target == sinkBlock) continue;
            if (context.number[target] != -1) continue;
            context.number[target] = static_cast<int>(context.order.size());
            context.order.push_back(target);
            queue.push(target);
        }
    }
}

// Создает минимальный автомат по финальному разбиению, перенося переходы от представителей классов.
Automaton Automaton::buildMinimizedAutomaton(
    const MinimizationContext& context) const {
    Automaton result;
    result.stateCount = static_cast<int>(context.order.size());
    result.alphabet = context.alphabet;
    const int sinkBlock = context.blockOf[context.sink];
    for (size_t i = 0; i < context.order.size(); ++i) {
        const int block = context.order[i];
        const int representative = context.blocks[block].front();
        for (int state : context.blocks[block]) {
            if (state != context.sink && finals.count(state)) {
                result.finals.insert(static_cast<int>(i));
                break;
            }
        }
        for (const std::string& symbol : context.alphabet) {
            const int target = context.blockOf[nextState(context, representative, symbol)];
            if (context.omitSink && target == sinkBlock) continue;
            result.transitions.push_back(
                {static_cast<int>(i), context.number[target], symbol});
        }
    }
    return result;
}

// Запускает этапы минимизации в порядке зависимостей: подготовка, разбиения, трассировка и построение результата.
Automaton Automaton::minimize() const {
    validate();
    MinimizationContext context = createContext();
    buildTransitionTable(context);
    findReachableStates(context);
    initializePartition(context);
    refinePartitions(context);
    printPartitionHistory(context);
    buildBlocks(context);
    numberBlocks(context);
    return buildMinimizedAutomaton(context);
}
