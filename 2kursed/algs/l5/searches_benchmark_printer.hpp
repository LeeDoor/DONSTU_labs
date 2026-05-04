#pragma once

#include "searches.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <ostream>
#include <random>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

class SearchesBenchmarkPrinter {
public:
    template <typename Benchmark, typename T = int, typename Compare = std::less<>>
    static void print_statistics_by_size(std::ostream& out, Compare compare = Compare()) {
        std::mt19937 generator(std::random_device{}());

        auto random_cases = build_random_cases<T>(generator, compare);

        out << "linear_search_vs_barrier" << '\n';
        print_table_header(out);

        for (const auto& array : random_cases) {
            for (const auto& search : linear_search_entries<T, Compare>()) {
                print_row_for_targets<Benchmark>(
                    out,
                    array.name,
                    search.name,
                    "average_present",
                    array.values,
                    array.present_targets,
                    search.function,
                    compare
                );

                print_row_for_target<Benchmark>(
                    out,
                    array.name,
                    search.name,
                    "best_begin",
                    array.values,
                    array.best_target,
                    search.function,
                    compare
                );

                print_row_for_target<Benchmark>(
                    out,
                    array.name,
                    search.name,
                    "worst_absent",
                    array.values,
                    array.worst_target,
                    search.function,
                    compare
                );
            }
        }

        out << '\n';

        auto sorted_cases = build_sorted_cases<T>(generator, compare);

        out << "linear_search_vs_binary_search_on_sorted_arrays" << '\n';
        print_table_header(out);

        for (const auto& array : sorted_cases) {
            for (const auto& search : sorted_search_entries<T, Compare>()) {
                print_row_for_targets<Benchmark>(
                    out,
                    array.name,
                    search.name,
                    "average_present",
                    array.values,
                    array.present_targets,
                    search.function,
                    compare
                );

                print_row_for_targets<Benchmark>(
                    out,
                    array.name,
                    search.name,
                    "average_absent",
                    array.values,
                    array.absent_targets,
                    search.function,
                    compare
                );
            }
        }

        out << '\n';
    }

private:
    struct empty_measurement {
        void operator()() const {}
    };

    template <typename Benchmark>
    using benchmark_time = decltype(Benchmark::measure(std::declval<empty_measurement>()));

    template <typename T, typename Compare>
    using search_function = search_stats (*)(
        typename std::vector<T>::iterator,
        typename std::vector<T>::iterator,
        const T&,
        Compare
    );

    template <typename T, typename Compare>
    struct search_entry {
        std::string_view name;
        search_function<T, Compare> function;
    };

    template <typename T>
    struct array_case {
        std::string name;
        std::vector<T> values;
        std::vector<T> present_targets;
        std::vector<T> absent_targets;
        T best_target;
        T worst_target;
    };

    static constexpr std::size_t measurements_count() {
        return 100;
    }

    static constexpr std::array<std::size_t, 6> benchmark_sizes() {
        return {20, 500, 1000, 3000, 5000, 10000};
    }

    template <typename T, typename Compare>
    static constexpr std::array<search_entry<T, Compare>, 2> linear_search_entries() {
        using iterator = typename std::vector<T>::iterator;

        return {{
            {"linear_search", &Searches::linear_search<iterator, T, Compare>},
            {"linear_search_with_barrier", &Searches::linear_search_with_barrier<iterator, T, Compare>}
        }};
    }

    template <typename T, typename Compare>
    static constexpr std::array<search_entry<T, Compare>, 2> sorted_search_entries() {
        using iterator = typename std::vector<T>::iterator;

        return {{
            {"linear_search", &Searches::linear_search<iterator, T, Compare>},
            {"binary_search", &Searches::binary_search<iterator, T, Compare>}
        }};
    }

    template <typename T, typename Compare>
    static std::vector<array_case<T>> build_random_cases(std::mt19937& generator, Compare compare) {
        std::vector<array_case<T>> cases;
        cases.reserve(benchmark_sizes().size());

        for (std::size_t size : benchmark_sizes()) {
            auto values = make_random_array<T>(size, generator);
            auto present_targets = make_present_targets(values, measurements_count(), generator);
            auto absent_targets = make_absent_targets(values, measurements_count(), generator, compare);
            T best_target = values.front();
            T worst_target = make_absent_target(values, generator, compare);

            cases.push_back({
                std::to_string(size),
                std::move(values),
                std::move(present_targets),
                std::move(absent_targets),
                best_target,
                worst_target
            });
        }

        return cases;
    }

    template <typename T, typename Compare>
    static std::vector<array_case<T>> build_sorted_cases(std::mt19937& generator, Compare compare) {
        std::vector<array_case<T>> cases;
        cases.reserve(benchmark_sizes().size());

        for (std::size_t size : benchmark_sizes()) {
            auto values = make_random_array<T>(size, generator);
            std::sort(values.begin(), values.end(), compare);

            auto present_targets = make_present_targets(values, measurements_count(), generator);
            auto absent_targets = make_absent_targets(values, measurements_count(), generator, compare);
            T best_target = values.front();
            T worst_target = make_absent_target(values, generator, compare);

            cases.push_back({
                std::to_string(size),
                std::move(values),
                std::move(present_targets),
                std::move(absent_targets),
                best_target,
                worst_target
            });
        }

        return cases;
    }

    template <typename T>
    static std::vector<T> make_random_array(std::size_t size, std::mt19937& generator) {
        std::vector<T> values(size);

        for (std::size_t index = 0; index < size; ++index) {
            values[index] = make_random_value<T>(generator);
        }

        return values;
    }

    template <typename T>
    static T make_random_value(std::mt19937& generator) {
        std::uniform_int_distribution<int> distribution(-1000000, 1000000);
        return static_cast<T>(distribution(generator));
    }

    template <typename T>
    static std::vector<T> make_present_targets(
        const std::vector<T>& values,
        std::size_t count,
        std::mt19937& generator
    ) {
        std::vector<T> targets;
        targets.reserve(count);

        std::uniform_int_distribution<std::size_t> distribution(0, values.size() - 1);

        for (std::size_t index = 0; index < count; ++index) {
            targets.push_back(values[distribution(generator)]);
        }

        return targets;
    }

    template <typename T, typename Compare>
    static std::vector<T> make_absent_targets(
        const std::vector<T>& values,
        std::size_t count,
        std::mt19937& generator,
        Compare compare
    ) {
        std::vector<T> targets;
        targets.reserve(count);

        while (targets.size() < count) {
            targets.push_back(make_absent_target(values, generator, compare));
        }

        return targets;
    }

    template <typename T, typename Compare>
    static T make_absent_target(
        const std::vector<T>& values,
        std::mt19937& generator,
        Compare compare
    ) {
        T target = make_random_value<T>(generator);

        while (contains_value(values, target, compare)) {
            target = make_random_value<T>(generator);
        }

        return target;
    }

    template <typename T, typename Compare>
    static bool contains_value(const std::vector<T>& values, const T& target, Compare compare) {
        return std::any_of(values.begin(), values.end(), [&](const T& value) {
            return equivalent(value, target, compare);
        });
    }

    template <typename T, typename Compare>
    static bool equivalent(const T& left, const T& right, Compare compare) {
        return !compare(left, right) && !compare(right, left);
    }

    static void print_table_header(std::ostream& out) {
        out << std::left
            << std::setw(16) << "size"
            << std::setw(32) << "algorithm"
            << std::setw(20) << "scenario"
            << std::setw(20) << "time"
            << std::setw(20) << "comparisons"
            << '\n';
    }

    template <typename SizeValue, typename TimeValue, typename ComparisonsValue>
    static void print_table_row(
        std::ostream& out,
        const SizeValue& size,
        std::string_view algorithm,
        std::string_view scenario,
        const TimeValue& time_value,
        const ComparisonsValue& comparisons
    ) {
        out << std::left
            << std::setw(16) << size
            << std::setw(32) << algorithm
            << std::setw(20) << scenario
            << std::setw(20) << time_value
            << std::setw(20) << comparisons
            << '\n';
    }

    template <typename Benchmark, typename RowLabel, typename T, typename Compare>
    static void print_row_for_target(
        std::ostream& out,
        const RowLabel& row_label,
        std::string_view algorithm,
        std::string_view scenario,
        const std::vector<T>& source,
        const T& target,
        search_function<T, Compare> function,
        Compare compare
    ) {
        std::vector<T> targets(measurements_count(), target);

        print_row_for_targets<Benchmark>(
            out,
            row_label,
            algorithm,
            scenario,
            source,
            targets,
            function,
            compare
        );
    }

    template <typename Benchmark, typename RowLabel, typename T, typename Compare>
    static void print_row_for_targets(
        std::ostream& out,
        const RowLabel& row_label,
        std::string_view algorithm,
        std::string_view scenario,
        const std::vector<T>& source,
        const std::vector<T>& targets,
        search_function<T, Compare> function,
        Compare compare
    ) {
        benchmark_time<Benchmark> total_time{};
        long double total_comparisons = 0;

        for (const auto& target : targets) {
            std::vector<T> values = source;
            search_stats stats{};

            total_time += Benchmark::measure([&]() {
                stats = function(values.begin(), values.end(), target, compare);
            });

            total_comparisons += static_cast<long double>(stats.comparisons);
        }

        auto average_time = total_time / static_cast<long long>(targets.size());
        auto average_comparisons = total_comparisons / static_cast<long double>(targets.size());

        print_table_row(
            out,
            row_label,
            algorithm,
            scenario,
            average_time,
            average_comparisons
        );
    }
};
