CFLAGS = -g -lc++

build/main: main.cpp build
	clang $(CFLAGS) main.cpp -o build/main

build:
	mkdir -p build

build/run: build
	mkdir -p build/run

build/run/tmp: build/run
	mkdir -p build/run/tmp

clean:
	rm -rf build

generate: build/run/1.in

build/run/1.in: build/run build/main
	build/main -g -o build/run/1.in

run: build/main build/run/1.in build/run/tmp
	build/main -i build/run/1.in -d build/run/tmp | less

debug: build/main tests/1.in
	lldb -- build/main

.PHONY: clean run debug test generate
