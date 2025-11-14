# Introduction

#todo

# Usage

```
// Generates a random.txt file with parameters specified, and exits
// parameters besides N, are only used internally to specify how big are the chunks written
<main> -g -N 100000 -b 10 -n 101 -o <filename>

// Sort the input file, using the temporary dir, and the parameters specified, save to the output file
<main> -i <input-file> -o <output-file> -d <tmp dir> -N 100000 -b 10 -n 101

// If no option is specified, system will prompt for parameters, and use the input data from stdin
<main>
```

- option `-i` implies the program is supposed to sort something, and thus is incompatible with `-g`
- when a parameter is not specified the default values are used:
  - i = "1.in"
  - o = "1.out"
  - d = "tmp"
  - g isn't set (false)
  - N = 100000
  - b = 10
  - n = 101

# Technical stuff

## memory alocated

- in total a b*m*sizeof(record) is alocated to memory for the buffers
  - in stage 1 it is used as a continuous block for reading, sorting, and writing
  - in stage 2 it is divided into b blocks, 1 for output, and b-1 for input, each block of size n
- an additional (b-1)\*2 bytes is alocated separately to create the heap (saved as a pointer to record + file index)
  - if could be carved out of the main memory block, but that would mean that buffers at stage 2 would need to be slightly smaller, and thus not cleanly divisible by n
  - the amount of memory for this heap is negligable in the scope of the entire memory usage anyway

## algorithm outline

### stage 1

In stage 1 the file is read in chunks of size b\*n, sorted in-memory, and saved as its own file
The amount of created runs is saved in memory

### stage 2

In stage 2 each buffer is populated by the HEAD of the coresponding file

- the HEAD pointer of each buffer is fed into a min heap, along with the computed g(record) value, and the buffer index
- when a pop is performed from the heap, another record is read from the same buffer, and the popped item written to output buffer
- when the output buffer is full, it is written to a file, subsequent writes are written to the same file
- when an input buffer is empty, more of the file is read into it
- when all input buffers are empty, the steps are repeated for any unprocessed input files
  The amount of runs is saved in memory, if more than one run reamain the process is repeated

## required functions

- [ ] split_buffer - splits the buffer into an array of buffers of size n
- [ ] heap_push, heap_pop
  - the heap is operating on structs of type g(record), index, buffer_index, and comparing them based on g(record)
  - [ ] push operation requires heapify function
- [ ] buffer i/o operations

# Quirks

the reader, writer, buffer, etc is a mess, could be much simpler, clearer, more readable, and less error prone if not for the "reading in chunks of bytes"

It creates a need for two separate buffers for reading, three if counting internal buffers of stream operations in c++. Unncessairly increases complexity, and so on.

I'm not really sure if i'm even doing it correctly and as intended, cause it's so messy

# Analysis

#todo
