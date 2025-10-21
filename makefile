CFLAGS = -g

build/main: build/main.o build/config.o build/buffer.o
	clang $(CFLAGS) build/main.o  build/config.o build/buffer.o -o build/main

build/main.o: main.c build
	clang $(CFLAGS) -c main.c -o build/main.o

build/config.o: config.c build
	clang $(CFLAGS) -c config.c -o build/config.o

build/buffer.o: buffer.c build
	clang $(CFLAGS) -c buffer.c -o build/buffer.o

build:
	mkdir -p build

build/run: build
	mkdir -p build/run

clean:
	rm -rf build

build/run/1.in: build/run build/main
	build/main -g -o build/run/1.in

run: build/main build/run/1.in
	build/main -i build/run/1.in | less

debug: build/main tests/1.in
	lldb -- build/main

.PHONY: clean run debug
