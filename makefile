CFLAGS = -g

build/main: build/main.o build/tape.o build
	clang $(CFLAGS) build/main.o build/tape.o -o build/main

build/main.o: main.c build
	clang $(CFLAGS) -c main.c -o build/main.o

build/tape.o: tape.c build
	clang $(CFLAGS) -c tape.c -o build/tape.o

build:
	mkdir -p build

tests:
	mkdir -p tests

clean:
	rm -rf build tests tmp

run: build/main tests/1.in
	build/main -f tests/1.in | less

debug: build/main tests/1.in
	lldb -- build/main -f tests/1.in

tests/1.in: build/main tests
	build/main -f tests/1.in -r  

.PHONY: clean run debug
