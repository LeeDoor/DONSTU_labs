CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRCS = Currency.cpp Dollar.cpp Euro.cpp Pound.cpp Purse.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = currency_program

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
