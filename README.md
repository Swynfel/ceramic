# Ceramic
A python module for playing variations of the board game Azul, implemented in C++.


## Build for Python

### Dependencies

Before building this repository make sure to install the following dependencies:
- C++14 compiler
- [Pybind11](https://github.com/pybind/pybind11)

#### Pybind11

[Pybind11](https://github.com/pybind/pybind11) is a library to binds the C++ code to python.

It can be installed with `pip` by executing the following command
```
python -m pip install pybind11
```

### Build

Execute the following command, at the root of the project
```
python setup.py build_ext --inplace
```
to generate the module.
For example, on linux, and using python3.7, it will be called `ceramic.cpython-37m-x86_64-linux-gnu.so` file.
It can safely be renamed `ceramic.so`.


## Build for C++ only

This part is to build c++ only (no python) libraries and executables.

### Dependencies

Make sure you have the following tools:
- CMake
- Make
- C++14 compiler

### Build

Execute the following:
```
mkdir build
cd build
cmake ..
make
```

If you want debugging symbols, replace `cmake ..` with
```
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

Executable and libraries will be placed in the `build` directory.

#### ceramic-test
Runs a test game with 4 random players.

#### ceramic-terminal-player
Play a game using the terminal.

To play a default game against 3 random players, call
```
./ceramic-terminal-player
```

To play against one random player and one first-legal player, using colors in the terminal, call
```
./ceramic-terminal-player rf -c f
```

For additional options, call with `-h` option.
```
./ceramic-terminal-player -h
```

You can type `-help` during the game to see action format.

#### ceramic-arena
Compare the performance of different agents.

Currently, no arguments can be passed.
The default will run a 1000 games with the default rules, for each possible games configuration (except for only the same player), out of the three players "first-legal", "random" with `smart=false`, and "random" with `smart=true`.
```
./ceramic-arena
```

It will output a recap table.
```
      player | winrate |  avg  |  std  
-------------+---------+-------+-------
 first-legal | 28.02 % |  20.0 |  11.1
random-naive |  2.26 % |   3.6 |   5.7
      random | 44.71 % |  25.0 |  13.7
```
- *avg* designates average score
- *std* designates the standard deviation of the score
