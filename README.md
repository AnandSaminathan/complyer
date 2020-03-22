# complyer

A tool which verifies if a model satisfies a desired property.

## Build instructions

Install [z3](https://github.com/Z3Prover/z3).

```
sudo apt-get install uuid-dev
sudo apt-get install antlr4
git clone https://github.com/AnandSaminathan/complyer
cd complyer
export LD_LIBRARY_PATH=`pwd`/libs
mkdir build
cd build
cmake ../
make
```

## Tool usage

After building, the tool can be used using.

```
./complyer [-int] <file-name>
```

Use [-int] option for interaction.
