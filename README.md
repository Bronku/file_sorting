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
