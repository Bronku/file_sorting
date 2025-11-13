CFLAGS = -g -lc++ -std=c++17

build/main: main.cpp sort.cpp config.hpp buffer.hpp reader.hpp writer.hpp build
	clang $(CFLAGS) main.cpp -o build/main

build:
	mkdir -p build

clean:
	rm -rf build

generate: build/run/1.in

# actually should depend on build/main and build/run, but that will trigger it every recompile, and i don't need that
build/run/1.in:
	build/main -g -o build/run/1.in

run: build/main build/run/1.in
	build/main -i build/run/1.in -d build/run/tmp

debug: build/main tests/1.in
	lldb -- build/main

.PHONY: clean run debug test generate
