#pragma once

#include <cstddef>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>

struct search_stats {
    std::size_t comparisons = 0;
    std::size_t position = std::numeric_limits<std::size_t>::max();
    bool found = false;
};

class Searches {
public:
    template <typename Iterator, typename T, typename Compare = std::less<>>
    static search_stats linear_search(Iterator first, Iterator last, const T& target, Compare compare = Compare()) {
        search_stats stats;
        std::size_t index = 0;

        for (auto current = first; current != last; ++current, ++index) {
            if (equivalent(*current, target, compare, stats)) {
                stats.found = true;
                stats.position = index;
                return stats;
            }
        }

        return stats;
    }

    template <typename Iterator, typename T, typename Compare = std::less<>>
    static search_stats linear_search_with_barrier(Iterator first, Iterator last, const T& target, Compare compare = Compare()) {
        search_stats stats;

        if (first == last) {
            return stats;
        }

        auto barrier = std::prev(last);
        auto saved_value = *barrier;
        const bool barrier_was_target = equivalent_without_count(saved_value, target, compare);

        *barrier = target;

        auto current = first;
        std::size_t index = 0;

        while (!equivalent(*current, target, compare, stats)) {
            ++current;
            ++index;
        }

        *barrier = std::move(saved_value);

        if (current != barrier || barrier_was_target) {
            stats.found = true;
            stats.position = index;
        }

        return stats;
    }

    template <typename Iterator, typename T, typename Compare = std::less<>>
    static search_stats binary_search(Iterator first, Iterator last, const T& target, Compare compare = Compare()) {
        search_stats stats;

        std::size_t left_index = 0;
        auto count = std::distance(first, last);

        while (count > 0) {
            auto step = count / 2;
            auto middle = first;
            std::advance(middle, step);

            std::size_t middle_index = left_index + static_cast<std::size_t>(step);

            if (equivalent(*middle, target, compare, stats)) {
                stats.found = true;
                stats.position = middle_index;
                return stats;
            }

            if (less_than(*middle, target, compare, stats)) {
                first = std::next(middle);
                left_index = middle_index + 1;
                count -= step + 1;
            } else {
                count = step;
            }
        }

        return stats;
    }

private:
    template <typename Left, typename Right, typename Compare>
    static bool equivalent(const Left& left, const Right& right, Compare compare, search_stats& stats) {
        ++stats.comparisons;
        return !compare(left, right) && !compare(right, left);
    }

    template <typename Left, typename Right, typename Compare>
    static bool less_than(const Left& left, const Right& right, Compare compare, search_stats& stats) {
        ++stats.comparisons;
        return compare(left, right);
    }

    template <typename Left, typename Right, typename Compare>
    static bool equivalent_without_count(const Left& left, const Right& right, Compare compare) {
        return !compare(left, right) && !compare(right, left);
    }
};
