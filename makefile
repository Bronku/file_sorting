CXX = clang++
CFLAGS = -g -lc++ -std=c++17

SRC := $(wildcard src/*.cpp)
TARGET = build/main

$(TARGET): $(SRC) | build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

build:
	mkdir -p build build/run

clean:
	rm -rf build

generate: build/run/1.in

# actually should depend on build/main and build/run, but that will trigger it every recompile, and i don't need that
build/run/1.in:
	$(TARGET) -g -o build/run/1.in

run: $(TARGET) build/run/1.in
	$(TARGET) -i build/run/1.in -d build/run/tmp

run_alt: $(TARGET) build/run/1.in
	$(TARGET) -i build/run/1.in -d build/run/tmp -n 11 -b 100

.PHONY: clean run generate run_alt
