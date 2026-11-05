#pragma once

#include <cstddef>
#include <iosfwd>
#include <set>
#include <string>
#include <vector>

struct Transition {
    int from;             // Индекс состояния, из которого выходит переход.
    int to;               // Индекс состояния, в которое ведет переход.
    std::string symbol;   // Символ алфавита, помечающий переход.
};

class Automaton {
public:
    int stateCount = 0;
    int start = 0;
    std::set<int> finals;
    std::set<std::string> alphabet;
    std::vector<Transition> transitions;

    // Записывает автомат в формате входного файла: число состояний, финальные состояния и переходы.
    void print(std::ostream& out) const;
    // Минимизирует автомат по последовательному уточнению классов эквивалентности и возвращает результат.
    Automaton minimize() const;

private:
    struct MinimizationContext;

    // Проверяет число состояний, начальное состояние и диапазоны индексов финальных состояний.
    void validate() const;
    // Создает рабочий контекст минимизации с дополнительным состоянием для отсутствующих переходов.
    MinimizationContext createContext() const;
    // Строит таблицу детерминированных переходов и включает в алфавит все встреченные символы.
    void buildTransitionTable(MinimizationContext& context) const;
    // Возвращает цель перехода; отсутствующий переход ведет в искусственное состояние sink.
    int nextState(const MinimizationContext& context, int state,
                  const std::string& symbol) const;
    // Обходит автомат от начального состояния и сохраняет все достижимые состояния, включая sink.
    void findReachableStates(MinimizationContext& context) const;
    // Формирует начальное разбиение на принимающие и непринимающие состояния и сохраняет его историю.
    void initializePartition(MinimizationContext& context) const;
    // Уточняет разбиение по классам состояний, достигаемых на каждом символе, до неподвижной точки.
    void refinePartitions(MinimizationContext& context) const;
    // Сравнивает разбиения по попарной эквивалентности состояний, не завися от номеров классов.
    bool samePartition(const std::vector<int>& first,
                       const std::vector<int>& second,
                       const std::vector<int>& states) const;
    // Выводит все сохраненные разбиения и явно сообщает, совпали ли два последних.
    void printPartitionHistory(const MinimizationContext& context) const;
    // Печатает одно разбиение как набор классов, выделяя начальное и финальные состояния цветом.
    void printPartition(const MinimizationContext& context, size_t order) const;
    // Выводит обозначение состояния с цветом начального/финального статуса; sink именуется отдельно.
    void printState(const MinimizationContext& context, int state) const;
    // Собирает список классов эквивалентности из окончательной нумерации состояний.
    void buildBlocks(MinimizationContext& context) const;
    // Нумерует классы обходом от начального, исключая отдельный класс sink из выходного автомата.
    void numberBlocks(MinimizationContext& context) const;
    // Создает итоговый автомат: переносит финальность классов и переходы их представителей.
    Automaton buildMinimizedAutomaton(const MinimizationContext& context) const;
};
