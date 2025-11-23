CXX = clang++
CXXFLAGS = -g -std=c++20 -Iinclude -Wall -Wextra -Werror
LDFLAGS =

SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:src/%.cpp=build/%.o)
TARGET = build/main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build build/run

clean:
	rm -rf build

.PHONY: clean
