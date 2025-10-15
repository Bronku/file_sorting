CFLAGS = -g

build/main: build/main.o build/tape.o build
	clang $(CFLAGS) build/main.o build/tape.o -o build/main

build/main.o: main.c build
	clang $(CFLAGS) -c main.c -o build/main.o

build/tape.o: tape.c build
	clang $(CFLAGS) -c tape.c -o build/tape.o

build:
	mkdir -p build

clean:
	rm -rf build

run: build/main
	build/main -f tests/1.in

debug: build/main
	lldb -- build/main -f tests/1.in

.PHONY: clean run 
