# complyer

A compiler which verifies if a model, given in the form of a program, satisfies some desired property given as input.

## Prerequisites
- Z3 : [Installation](https://github.com/Z3Prover/z3)
- CMake : ```sudo apt-get install cmake```
- Package Config : ```sudo apt-get install pkg-config```
- UUID : ```sudo apt-get install uuid-dev```
- ANTLR4 : ```sudo apt-get install antlr4```

## Build Instructions
Navigate to root directory of the repository
```
export LD_LIBRARY_PATH=`pwd`/libs
mkdir build; cd build
cmake ../
make
```

## Tool usage

After building, the tool can be used using.

```
./complyer [OPTIONS] <file-name>
```
#### Options
- ```-i, --interactive``` : Enable Interaction
- ```-v, --verbose``` : Descriptive Logs
