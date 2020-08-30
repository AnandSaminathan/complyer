# complyer

A compiler which verifies if a model, given in the form of a program, satisfies some desired property given as input.

## Prerequisites
- Z3 : [Installation](https://github.com/Z3Prover/z3)
- CMake : ```sudo apt-get install cmake```
- Package Config : ```sudo apt-get install pkg-config```
- UUID : ```sudo apt-get install uuid-dev```
- ANTLR4 : ```sudo apt-get install antlr4```

## Dependencies
- [Verification Algorithms](https://github.com/AnandSaminathan/verification-algorithms)
- [Formula Tree](https://github.com/AnandSaminathan/formula-tree)

## Build Instructions
Navigate to root directory of the repository
```
export LD_LIBRARY_PATH=`pwd`/build/formulatree/src/git_formulatree/bin
mkdir build; cd build
cmake ../
make
```

## Tool usage

After building, the tool can be run using.

```
./complyer [-i] [-v] [-b <command-file>] <file-name>
```
#### Options
- ```-i, --interactive``` : Enable Interaction
- ```-v, --verbose``` : Descriptive Logs
- ```-b, --batch``` : Batch process commands

**Note:** Interactive and Batch cannot be enabled simultaneously
