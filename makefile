CFLAGS = -g

build/main: build/main.o build/config.o build/buffer.o build/heap.o
	clang $(CFLAGS) build/main.o  build/config.o build/buffer.o build/heap.o -o build/main

build/main.o: main.c build
	clang $(CFLAGS) -c main.c -o build/main.o

build/config.o: config.c build
	clang $(CFLAGS) -c config.c -o build/config.o

build/buffer.o: buffer.c build
	clang $(CFLAGS) -c buffer.c -o build/buffer.o

build/heap.o: heap.c build
	clang $(CFLAGS) -c heap.c -o build/heap.o

build:
	mkdir -p build

build/run: build
	mkdir -p build/run

build/run/tmp: build/run
	mkdir -p build/run/tmp

clean:
	rm -rf build

build/run/1.in: build/run build/main
	build/main -g -o build/run/1.in

run: build/main build/run/1.in build/run/tmp
	build/main -i build/run/1.in -d build/run/tmp | less

build/heap_test.o: heap_test.c build
	clang $(CFLAGS) -c heap_test.c -o build/heap_test.o

build/test: build/heap_test.o build/heap.o
	clang $(CFLAGS) build/heap_test.o build/heap.o -o build/test

test: build/test
	build/test

debug: build/main tests/1.in
	lldb -- build/main

.PHONY: clean run debug test
