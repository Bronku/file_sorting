# Generate 5 significantly different file sizes

echo "./build/main -g 1000 -o build/run/tests/test_1k.in"
./build/main -g 1000 -o build/run/tests/test_1k.in -d build/run/tmp
echo "./build/main -g 5000 -o build/run/tests/test_5k.in"
./build/main -g 5000 -o build/run/tests/test_5k.in -d build/run/tmp
echo "./build/main -g 10000 -o build/run/tests/test_10k.in"
./build/main -g 10000 -o build/run/tests/test_10k.in -d build/run/tmp
echo "./build/main -g 50000 -o build/run/tests/test_50k.in"
./build/main -g 50000 -o build/run/tests/test_50k.in -d build/run/tmp
echo "./build/main -g 100000 -o build/run/tests/test_100k.in"
./build/main -g 100000 -o build/run/tests/test_100k.in -d build/run/tmp

# Sort each test file and capture statistics

echo "./build/main -i build/run/tests/test_1k.in -o build/run/tests/test_1k.out -n 4 -b 100"
./build/main -i build/run/tests/test_1k.in -o build/run/tests/test_1k.out -n 4 -b 100 -d build/run/tmp
echo "./build/main -i build/run/tests/test_5k.in -o build/run/tests/test_5k.out -n 4 -b 100"
./build/main -i build/run/tests/test_5k.in -o build/run/tests/test_5k.out -n 4 -b 100 -d build/run/tmp
echo "./build/main -i build/run/tests/test_10k.in -o build/run/tests/test_10k.out -n 4 -b 100"
./build/main -i build/run/tests/test_10k.in -o build/run/tests/test_10k.out -n 4 -b 100 -d build/run/tmp
echo "./build/main -i build/run/tests/test_50k.in -o build/run/tests/test_50k.out -n 4 -b 100"
./build/main -i build/run/tests/test_50k.in -o build/run/tests/test_50k.out -n 4 -b 100 -d build/run/tmp
echo "./build/main -i build/run/tests/test_100k.in -o build/run/tests/test_100k.out -n 4 -b 100"
./build/main -i build/run/tests/test_100k.in -o build/run/tests/test_100k.out -n 4 -b 100 -d build/run/tmp

# Display first/last records to verify sorting

echo "./build/main -e -i build/run/tests/test_1k.out | less"
./build/main -e -i build/run/tests/test_1k.out | less -d build/run/tmp
echo "./build/main -e -i build/run/tests/test_100k.out | less"
./build/main -e -i build/run/tests/test_100k.out | less -d build/run/tmp

# Test with different K values (buffer rows)

echo "./build/main -i build/run/tests/test_10k.in -o build/run/tests/test_10k_k2.out -n 3 -b 100 # K=2"
./build/main -i build/run/tests/test_10k.in -o build/run/tests/test_10k_k2.out -n 3 -b 100 # K=2 -d build/run/tmp
echo "./build/main -i build/run/tests/test_10k.in -o build/run/tests/test_10k_k4.out -n 5 -b 100 # K=4"
./build/main -i build/run/tests/test_10k.in -o build/run/tests/test_10k_k4.out -n 5 -b 100 # K=4 -d build/run/tmp
echo "./build/main -i build/run/tests/test_10k.in -o build/run/tests/test_10k_k8.out -n 9 -b 100 # K=8"
./build/main -i build/run/tests/test_10k.in -o build/run/tests/test_10k_k8.out -n 9 -b 100 # K=8 -d build/run/tmp
