#include "searches_benchmark_printer.hpp"
#include "benchmark.hpp"

#include <exception>
#include <fstream>
#include <iostream>

int main() {
    try {
        SearchesBenchmarkPrinter::print_statistics_by_size<Benchmark>(std::cout);

        std::ofstream file("searches_benchmark.txt");

        if (!file) {
            std::cerr << "Failed to open output file\n";
            return 1;
        }

        SearchesBenchmarkPrinter::print_statistics_by_size<Benchmark>(file);

        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
