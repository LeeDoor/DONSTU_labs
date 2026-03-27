#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

struct sort_stats {
    std::size_t iterations = 0;
    std::size_t comparisons = 0;
    std::size_t swaps = 0;

    sort_stats& operator+=(const sort_stats& other) {
        iterations += other.iterations;
        comparisons += other.comparisons;
        swaps += other.swaps;
        return *this;
    }
};

template <typename Iterator>
concept sorting_iterator = requires(
    Iterator it,
    Iterator other,
    typename std::iterator_traits<Iterator>::difference_type offset
) {
    typename std::iterator_traits<Iterator>::value_type;
    typename std::iterator_traits<Iterator>::difference_type;
    typename std::iterator_traits<Iterator>::reference;

    { *it } -> std::same_as<typename std::iterator_traits<Iterator>::reference>;

    { ++it } -> std::same_as<Iterator&>;
    { it++ } -> std::same_as<Iterator>;
    { --it } -> std::same_as<Iterator&>;
    { it-- } -> std::same_as<Iterator>;

    { it == other } -> std::convertible_to<bool>;
    { it != other } -> std::convertible_to<bool>;

    { it + offset } -> std::same_as<Iterator>;
    { offset + it } -> std::same_as<Iterator>;
    { it - offset } -> std::same_as<Iterator>;
    { it - other } -> std::same_as<typename std::iterator_traits<Iterator>::difference_type>;

    { std::iter_swap(it, other) } -> std::same_as<void>;
};

class Sortings {
public:
    template <sorting_iterator Iterator, typename Compare = std::less<>>
    static sort_stats direct_insertion_sort(Iterator first, Iterator last, Compare compare = Compare()) {
        sort_stats stats;
        if (first == last) {
            return stats;
        }

        auto counted_compare = make_counted_compare(stats, compare);

        for (Iterator current = std::next(first); current != last; ++current) {
            ++stats.iterations;
            Iterator position = current;

            while (position != first) {
                ++stats.iterations;
                Iterator previous = std::prev(position);
                if (!counted_compare(*position, *previous)) {
                    break;
                }
                counted_iter_swap(position, previous, stats);
                position = previous;
            }
        }

        return stats;
    }

    template <sorting_iterator Iterator, typename Compare = std::less<>>
    static sort_stats direct_selection_sort(Iterator first, Iterator last, Compare compare = Compare()) {
        sort_stats stats;
        auto counted_compare = make_counted_compare(stats, compare);

        for (Iterator current = first; current != last; ++current) {
            ++stats.iterations;
            Iterator minimum = current;

            for (Iterator probe = std::next(current); probe != last; ++probe) {
                ++stats.iterations;
                if (counted_compare(*probe, *minimum)) {
                    minimum = probe;
                }
            }

            if (minimum != current) {
                counted_iter_swap(current, minimum, stats);
            }
        }

        return stats;
    }

    template <sorting_iterator Iterator, typename Compare = std::less<>>
    static sort_stats direct_exchange_sort(Iterator first, Iterator last, Compare compare = Compare()) {
        sort_stats stats;
        if (first == last) {
            return stats;
        }

        auto counted_compare = make_counted_compare(stats, compare);
        bool swapped = true;
        Iterator unsorted_end = last;

        while (swapped) {
            ++stats.iterations;
            swapped = false;
            Iterator current = first;
            Iterator next_it = std::next(first);

            if (next_it == unsorted_end) {
                break;
            }

            while (next_it != unsorted_end) {
                ++stats.iterations;
                if (counted_compare(*next_it, *current)) {
                    counted_iter_swap(current, next_it, stats);
                    swapped = true;
                }
                ++current;
                ++next_it;
            }

            unsorted_end = current;
        }

        return stats;
    }

    template <sorting_iterator Iterator, typename Compare = std::less<>>
    static sort_stats hoare_sort(Iterator first, Iterator last, Compare compare = Compare()) {
        sort_stats stats;
        auto length = last - first;
        if (length > 1) {
            auto counted_compare = make_counted_compare(stats, compare);
            hoare_sort_impl(first, 0, length - 1, stats, counted_compare);
        }
        return stats;
    }

    template <sorting_iterator Iterator, typename Compare = std::less<>>
        requires requires(typename std::iterator_traits<Iterator>::reference value) {
            { value % 3 } -> std::convertible_to<typename std::iterator_traits<Iterator>::value_type>;
        }
    static sort_stats direct_selection_sort_divisible_by_3(Iterator first, Iterator last, Compare compare = Compare()) {
        sort_stats stats;
        auto counted_compare = make_counted_compare(stats, compare);

        for (Iterator current = first; current != last; ++current) {
            ++stats.iterations;
            if (!is_divisible_by_3(*current)) {
                continue;
            }

            Iterator minimum = current;

            for (Iterator probe = std::next(current); probe != last; ++probe) {
                ++stats.iterations;
                if (!is_divisible_by_3(*probe)) {
                    continue;
                }
                if (counted_compare(*probe, *minimum)) {
                    minimum = probe;
                }
            }

            if (minimum != current) {
                counted_iter_swap(current, minimum, stats);
            }
        }

        return stats;
    }

private:
    template <typename Compare>
    class comparison_wrapper {
    public:
        comparison_wrapper(sort_stats& stats, Compare compare)
            : stats_(stats), compare_(std::move(compare)) {
        }

        template <typename Left, typename Right>
        bool operator()(Left&& lhs, Right&& rhs) {
            ++stats_.comparisons;
            return static_cast<bool>(compare_(std::forward<Left>(lhs), std::forward<Right>(rhs)));
        }

    private:
        sort_stats& stats_;
        Compare compare_;
    };

    template <typename Compare>
    static comparison_wrapper<Compare> make_counted_compare(sort_stats& stats, Compare compare) {
        return comparison_wrapper<Compare>(stats, std::move(compare));
    }

    template <typename Iterator>
    static void counted_iter_swap(Iterator lhs, Iterator rhs, sort_stats& stats) {
        std::iter_swap(lhs, rhs);
        ++stats.swaps;
    }

    template <typename Value>
    static bool is_divisible_by_3(const Value& value) {
        return value % 3 == 0;
    }

    template <sorting_iterator Iterator, typename CountedCompare>
    static typename std::iterator_traits<Iterator>::difference_type hoare_partition(
        Iterator first,
        typename std::iterator_traits<Iterator>::difference_type low,
        typename std::iterator_traits<Iterator>::difference_type high,
        sort_stats& stats,
        CountedCompare& compare
    ) {
        using difference_type = typename std::iterator_traits<Iterator>::difference_type;

        auto pivot = *(first + (low + high) / 2);
        difference_type left = low - 1;
        difference_type right = high + 1;

        while (true) {
            do {
                ++left;
                ++stats.iterations;
            } while (compare(*(first + left), pivot));

            do {
                --right;
                ++stats.iterations;
            } while (compare(pivot, *(first + right)));

            ++stats.iterations;
            if (left >= right) {
                return right;
            }

            counted_iter_swap(first + left, first + right, stats);
        }
    }

    template <sorting_iterator Iterator, typename CountedCompare>
    static void hoare_sort_impl(
        Iterator first,
        typename std::iterator_traits<Iterator>::difference_type low,
        typename std::iterator_traits<Iterator>::difference_type high,
        sort_stats& stats,
        CountedCompare& compare
    ) {
        if (low >= high) {
            return;
        }

        auto partition_index = hoare_partition(first, low, high, stats, compare);
        hoare_sort_impl(first, low, partition_index, stats, compare);
        hoare_sort_impl(first, partition_index + 1, high, stats, compare);
    }
};

