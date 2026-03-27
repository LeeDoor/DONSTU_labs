#pragma once

#include "sortings.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <iomanip>
#include <ostream>
#include <random>
#include <string_view>
#include <utility>
#include <vector>

class SortingsBenchmarkPrinter {
public:
    template <typename Benchmark, typename T = int, typename Compare = std::less<>>
    static void print_statistics_by_size(std::ostream& out, Compare compare = Compare()) {
        std::mt19937 generator(std::random_device{}());

        for (const auto& sorting : sorting_entries<T, Compare>()) {
            out << sorting.name << '\n';
            print_table_header(out, "size");

            for (std::size_t size : benchmark_sizes()) {
                auto source = make_random_array<T>(size, generator);
                print_row_for_source<Benchmark>(out, size, source, sorting.function, compare);
            }

            out << '\n';
        }
    }

    template <typename Benchmark, typename T = int, typename Compare = std::less<>>
    static void print_statistics_by_array_type(std::ostream& out, std::size_t size = 1000, Compare compare = Compare()) {
        std::mt19937 generator(std::random_device{}());
        auto cases = build_array_cases<T>(size, generator);

        for (const auto& sorting : sorting_entries<T, Compare>()) {
            out << sorting.name << '\n';
            print_table_header(out, "array_type");

            for (const auto& array_case : cases) {
                print_row_for_source<Benchmark>(out, array_case.name, array_case.values, sorting.function, compare);
            }

            out << '\n';
        }
    }

private:
    template <typename T, typename Compare>
    using sort_function = sort_stats (*)(typename std::vector<T>::iterator, typename std::vector<T>::iterator, Compare);

    template <typename T, typename Compare>
    struct sorting_entry {
        std::string_view name;
        sort_function<T, Compare> function;
    };

    template <typename T>
    struct array_case {
        std::string_view name;
        std::vector<T> values;
    };

    static constexpr std::array<std::size_t, 6> benchmark_sizes() {
        return {20, 500, 1000, 3000, 5000, 10000};
    }

    template <typename T, typename Compare>
    static constexpr std::array<sorting_entry<T, Compare>, 4> sorting_entries() {
        using iterator = typename std::vector<T>::iterator;
        return {{
            {"direct_insertion_sort", &Sortings::direct_insertion_sort<iterator, Compare>},
            {"direct_selection_sort", &Sortings::direct_selection_sort<iterator, Compare>},
            {"direct_exchange_sort", &Sortings::direct_exchange_sort<iterator, Compare>},
            {"hoare_sort", &Sortings::hoare_sort<iterator, Compare>}
        }};
    }

    template <typename T>
    static std::vector<array_case<T>> build_array_cases(std::size_t size, std::mt19937& generator) {
        std::vector<array_case<T>> cases;
        cases.reserve(4);
        cases.push_back({"random", make_random_array<T>(size, generator)});
        cases.push_back({"sorted", make_sorted_array<T>(size, generator)});
        cases.push_back({"reversed", make_reversed_array<T>(size, generator)});
        cases.push_back({"sorted 25%", make_partially_sorted_array<T>(size, generator, .25)});
        cases.push_back({"sorted 50%", make_partially_sorted_array<T>(size, generator, .50)});
        cases.push_back({"sorted 75%", make_partially_sorted_array<T>(size, generator, .75)});
        return cases;
    }

    template <typename T>
    static std::vector<T> make_random_array(std::size_t size, std::mt19937& generator) {
        std::uniform_int_distribution<int> distribution(-1000000, 1000000);
        std::vector<T> values(size);
        for (std::size_t index = 0; index < size; ++index) {
            values[index] = static_cast<T>(distribution(generator));
        }
        return values;
    }

    template <typename T>
    static std::vector<T> make_sorted_array(std::size_t size, std::mt19937& generator) {
        auto values = make_random_array<T>(size, generator);
        std::sort(values.begin(), values.end());
        return values;
    }

    template <typename T>
    static std::vector<T> make_reversed_array(std::size_t size, std::mt19937& generator) {
        auto values = make_sorted_array<T>(size, generator);
        std::reverse(values.begin(), values.end());
        return values;
    }

    template <typename T>
    static std::vector<T> make_partially_sorted_array(std::size_t size, std::mt19937& generator, double part) {
        auto values = make_random_array<T>(size, generator);
        std::size_t sorted_part = size * part;
        std::sort(values.begin(), values.begin() + static_cast<std::ptrdiff_t>(sorted_part));
        return values;
    }

    static void print_table_header(std::ostream& out, std::string_view first_column_name) {
        out << std::left
            << std::setw(20) << first_column_name
            << std::setw(20) << "time"
            << std::setw(20) << "iterations"
            << std::setw(20) << "comparisons"
            << std::setw(20) << "swaps"
            << '\n';
    }

    template <typename FirstValue, typename TimeValue>
    static void print_table_row(std::ostream& out, const FirstValue& first_value, const TimeValue& time_value, const sort_stats& stats) {
        out << std::left
            << std::setw(20) << first_value
            << std::setw(20) << time_value
            << std::setw(20) << stats.iterations
            << std::setw(20) << stats.comparisons
            << std::setw(20) << stats.swaps
            << '\n';
    }

    template <typename Benchmark, typename RowLabel, typename T, typename Compare>
    static void print_row_for_source(
        std::ostream& out,
        const RowLabel& row_label,
        const std::vector<T>& source,
        sort_function<T, Compare> sorting_function,
        Compare compare
    ) {
        std::vector<T> values;
        sort_stats stats;
        auto time_value = Benchmark::measure([&]() {
            values = source;
            stats = sorting_function(values.begin(), values.end(), compare);
        });
        print_table_row(out, row_label, time_value, stats);
    }
};

