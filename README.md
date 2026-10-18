# super-duper-rda-algorithm
RDA algorithm with multi-GPU support

Google Test is used for testing purposes.

Install dependencies:

```
sudo apt-get update && sudo apt-get install -y libgtest-dev libgmock-dev
```

Compile and run test cases:

```
g++ test.cpp -o test -I. -lgtest -lgmock -lpthread && ./test
```

Compile and run the program.

```
g++ main.cpp -o main -I. && ./main
```
