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
