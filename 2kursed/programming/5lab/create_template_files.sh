#!/bin/bash

# Создаем файл fibonacci.hpp с шаблонами
cat > fibonacci.hpp << 'EOF'
#pragma once

#include <iostream>
#include <vector>
#include <string>

// 1. Шаблон функции для генерации n чисел Фибоначчи
template <typename T>
std::vector<T> generateFibonacci(int n) {
    std::vector<T> result;
    if (n <= 0) return result;
    
    if (n >= 1) result.push_back(0);
    if (n >= 2) result.push_back(1);
    
    for (int i = 2; i < n; i++) {
        T next = result[i-1] + result[i-2];
        result.push_back(next);
    }
    
    return result;
}

// 3. Специализация функции для работы с short
template <>
std::vector<short> generateFibonacci<short>(int n) {
    std::vector<short> result;
    if (n <= 0) return result;
    
    if (n >= 1) result.push_back(0);
    if (n >= 2) result.push_back(1);
    
    for (int i = 2; i < n; i++) {
        short next = result[i-1] + result[i-2];
        if (next < result[i-1]) { // Проверка на переполнение
            std::cout << "Warning: overflow detected for short at position " << i << std::endl;
            break;
        }
        result.push_back(next);
    }
    
    return result;
}

// 2. Шаблонный класс, который хранит последовательность Фибоначчи
template <typename T>
class FibonacciSequence {
private:
    std::vector<T> sequence;
public:
    FibonacciSequence(int n) {
        sequence = generateFibonacci<T>(n);
    }
    
    void print() const {
        std::cout << "Fibonacci sequence: ";
        for (size_t i = 0; i < sequence.size(); i++) {
            std::cout << sequence[i];
            if (i < sequence.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    
    std::vector<T> getSequence() const {
        return sequence;
    }
    
    size_t size() const {
        return sequence.size();
    }
};

// 4. Специализация класса для работы с типом int
template <>
class FibonacciSequence<int> {
private:
    std::vector<int> sequence;
public:
    FibonacciSequence(int n) {
        sequence = generateFibonacci<int>(n);
    }
    
    void print() const {
        std::cout << "Fibonacci sequence (int specialized): ";
        for (size_t i = 0; i < sequence.size(); i++) {
            std::cout << sequence[i];
            if (i < sequence.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    
    double getAverage() const {
        if (sequence.empty()) return 0.0;
        
        long long sum = 0;
        for (int num : sequence) {
            sum += num;
        }
        return static_cast<double>(sum) / sequence.size();
    }
    
    long long getSum() const {
        long long sum = 0;
        for (int num : sequence) {
            sum += num;
        }
        return sum;
    }
    
    std::vector<int> getSequence() const {
        return sequence;
    }
    
    size_t size() const {
        return sequence.size();
    }
    
    void printStatistics() const {
        std::cout << "Statistics for int sequence:" << std::endl;
        std::cout << "Size: " << size() << std::endl;
        std::cout << "Sum: " << getSum() << std::endl;
        std::cout << "Average: " << getAverage() << std::endl;
    }
};
EOF

# Создаем файл main.cpp с демонстрационной программой
cat > main.cpp << 'EOF'
#include "fibonacci.hpp"
#include <iostream>
#include <vector>

int main() {
    std::cout << "=== Fibonacci Template Demonstration ===" << std::endl;
    std::cout << std::endl;
    
    // 1. Использование шаблонной функции с разными типами
    std::cout << "1. Testing template function with different types:" << std::endl;
    
    auto fibInt = generateFibonacci<int>(10);
    std::cout << "Fibonacci<int>(10): ";
    for (int num : fibInt) std::cout << num << " ";
    std::cout << std::endl;
    
    auto fibLong = generateFibonacci<long long>(10);
    std::cout << "Fibonacci<long long>(10): ";
    for (long long num : fibLong) std::cout << num << " ";
    std::cout << std::endl;
    
    auto fibDouble = generateFibonacci<double>(10);
    std::cout << "Fibonacci<double>(10): ";
    for (double num : fibDouble) std::cout << num << " ";
    std::cout << std::endl;
    
    std::cout << std::endl;
    
    // 3. Специализация для short с обработкой переполнения
    std::cout << "2. Testing specialization for short:" << std::endl;
    auto fibShort = generateFibonacci<short>(20);
    std::cout << "Fibonacci<short>(20): ";
    for (short num : fibShort) std::cout << num << " ";
    std::cout << std::endl;
    
    std::cout << std::endl;
    
    // 2. Использование шаблонного класса
    std::cout << "3. Testing template class:" << std::endl;
    
    FibonacciSequence<long> fibSeq1(15);
    std::cout << "FibonacciSequence<long>(15):" << std::endl;
    fibSeq1.print();
    
    FibonacciSequence<double> fibSeq2(8);
    std::cout << "FibonacciSequence<double>(8):" << std::endl;
    fibSeq2.print();
    
    std::cout << std::endl;
    
    // 4. Использование специализированного класса для int
    std::cout << "4. Testing specialized class for int:" << std::endl;
    
    FibonacciSequence<int> fibSeqInt(15);
    fibSeqInt.print();
    fibSeqInt.printStatistics();
    
    std::cout << std::endl;
    
    // Демонстрация с разными размерами последовательностей
    std::cout << "5. Testing different sequence sizes:" << std::endl;
    
    FibonacciSequence<int> smallSeq(5);
    std::cout << "Sequence of size 5:" << std::endl;
    smallSeq.print();
    
    FibonacciSequence<int> emptySeq(0);
    std::cout << "Sequence of size 0:" << std::endl;
    emptySeq.print();
    
    std::cout << std::endl;
    
    // Тестирование с другими типами данных
    std::cout << "6. Additional tests with different types:" << std::endl;
    
    FibonacciSequence<unsigned int> fibUnsigned(10);
    std::cout << "FibonacciSequence<unsigned int>(10):" << std::endl;
    fibUnsigned.print();
    
    std::cout << std::endl;
    std::cout << "=== Demonstration Completed ===" << std::endl;
    
    return 0;
}
EOF

# Создаем Makefile для компиляции
cat > Makefile << 'EOF'
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = fibonacci_demo

all: $(TARGET)

$(TARGET): main.cpp fibonacci.hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	@echo "Running fibonacci demo..."
	./$(TARGET)

.PHONY: all clean run test
EOF

# Даем права на выполнение
chmod +x create_template_files.sh
chmod +x Makefile
