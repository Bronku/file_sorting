build/main: main.c build
	clang main.c -o build/main

build:
	mkdir -p build

clean:
	rm -rf build

run: build/main
	build/main -f tests/1.in

.PHONY: clean run 
