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
