#include "sorting_benchmark.hpp"
#include "benchmark.hpp"

#include <chrono>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

std::optional<std::size_t> get_sizet(const std::string& str) {
    if (str.empty()) {
        return std::nullopt;
    }
    if (str.find_first_not_of("0123456789") != std::string::npos) {
        return std::nullopt;
    }
    if (str[0] == '0') {
        return std::nullopt;
    }
    return static_cast<std::size_t>(std::stoull(str));
}

std::size_t get_k() {
    std::string input;
    std::optional<std::size_t> converted;
    do {
        std::cout << "Enter positive integer K: ";
        std::getline(std::cin, input);
        converted = get_sizet(input);
    } while (!converted || *converted == 0);
    return *converted;
}

std::vector<int> get_array_from_user() {
    std::cout << "Enter array elements separated by spaces: ";

    std::string line;
    std::getline(std::cin, line);

    std::istringstream stream(line);
    std::vector<int> values;
    int value = 0;

    while (stream >> value) {
        values.push_back(value);
    }
    
    if(!stream.eof()) {
	std::cout << "Error while forming an array" << std::endl;
	return {};
    }

    return values;
}

template <typename T>
void print_array(const std::vector<T>& values) {
    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index != 0) {
            std::cout << ' ';
        }
        std::cout << values[index];
    }
    std::cout << '\n';
}

int main() {
    std::vector<int> values = get_array_from_user();
    if(!values.empty()) {
	std::cout << "array before sorting divisible by 3 elements\n";
	print_array(values);

	const sort_stats special_stats = Sortings::direct_selection_sort_divisible_by_3(values.begin(), values.end());

	std::cout << "array after sorting divisible by 3 elements\n";
	print_array(values);
	std::cout << "iterations: " << special_stats.iterations
		  << ", comparisons: " << special_stats.comparisons
		  << ", swaps: " << special_stats.swaps << "\n\n";

    }
    std::cout << "statistics by size\n\n";
    SortingsBenchmarkPrinter::print_statistics_by_size<Benchmark>(std::cout);

    const std::size_t k = get_k();

    std::cout << "statistics by array type\n\n";
    SortingsBenchmarkPrinter::print_statistics_by_array_type<Benchmark>(std::cout, k);

    return 0;
}

