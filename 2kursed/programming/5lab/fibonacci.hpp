#pragma once
#include <iostream>
#include <vector>

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

template <>
std::vector<short> generateFibonacci<short>(int n) {
    std::vector<short> result;
    if (n <= 0) return result;
    
    if (n >= 1) result.push_back(0);
    if (n >= 2) result.push_back(1);
    
    for (int i = 2; i < n; i++) {
        short next = result[i-1] + result[i-2];
        if (next < result[i-1]) { 
            std::cout << "Warning: overflow detected for short at position " << i << std::endl;
            break;
        }
        result.push_back(next);
    }
    
    return result;
}

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
