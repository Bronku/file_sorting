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

generate: build/run/1.in

# actually should depend on build/main and build/run, but that will trigger it every recompile, and i don't need that
build/run/1.in:
	$(TARGET) -g 100000 -o build/run/1.in

run: $(TARGET) build/run/1.in
	$(TARGET) -i build/run/1.in -d build/run/tmp -o build/run/1.out

run_alt: $(TARGET) build/run/1.in
	$(TARGET) -i build/run/1.in -d build/run/tmp -n 11 -b 100

.PHONY: clean run generate run_alt all
