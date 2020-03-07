# complyer

A tool which verifies if a model satisfies a desired property.

## Build instructions

Install [z3](https://github.com/Z3Prover/z3).

```
sudo apt-get install uuid-dev
sudo apt-get install antlr4
mkdir build
cd build
cmake ../
make
```

## Tool usage

After building, the tool can be used using.

```
./complyer <file-name>
```
