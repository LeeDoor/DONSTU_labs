#include "get_valid_input.hpp"
#include <algorithm>
#include <chrono>
#include <list>

using Strings = std::list<std::string>;

std::string number_to_binary_str(int N) {
	std::string result;
	if (N == 0)
		return "0";
	while (N) {
		result.push_back(N % 2 == 0 ? '0' : '1');
		N /= 2;
	}
	std::reverse(result.begin(), result.end());
	return result;
}

Strings generate_strings(int N) {
	Strings zeros{}, ones {};
    int maxl = number_to_binary_str(N).size();
	for (int i = 0; i < N; ++i) {
		std::string bin(number_to_binary_str(i));
        std::string filled(maxl - bin.size(), '0');
        zeros.push_back('0' + filled + bin + '0');
        ones.push_back('1' + filled + bin + '1');
	}
    zeros.merge(ones);
    return zeros;
}

int main() {
	std::cout << "Enter positive N: ";
	int N = get_valid_input(1);
	auto start = std::chrono::high_resolution_clock::now();
	Strings strings = generate_strings(N);
	auto generated = std::chrono::high_resolution_clock::now();
	std::cout << "generated for: " << std::chrono::duration_cast<std::chrono::milliseconds>(generated - start).count()
	          << std::endl;
	std::cin.get();
	int i = 0;
	for (auto string : strings) {
		std::cout << i++ << ": " << string << std::endl;
	}
}
