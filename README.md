# Ceramic
A python module for playing variations of the board game Azul, implemented in C++.

## Build using python's setuptools

### Dependencies

The simplest method for building the python module is using python's `setuptools`.
Make sure to install the following dependencies:
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


### Test

To test the project works as intended, you can run the list of tests in the `test/` directory.

The recommended method is to install [tox](https://) with `pip install tox`, and execute in the root of the repository
```
tox
```

If everything works as expected, you should see an output similar to
```
============================ 127 passed in 0.17s =============================
__________________________________ summary ___________________________________
  py37: commands succeeded
  congratulations :)
```

## Build using CMake

Although using CMake is slightly less straight forward, it allows more fine-tuning.
For example, you can build c++ only libraries and executables, without the python module.

### Dependencies

Make sure you have the following tools:
- CMake
- Make
- C++14 compiler
- [Pybind11](https://github.com/pybind/pybind11) (optional, only required for python module)

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

#### Build python module

If you want to build the python module for python version 3.x, start by installing the corresponding pybind headers.
Then export `PYTHON=3.x` before calling the `cmake` command
```
export PYTHON=3.x #Unix
SET PYTHON=3.x #Windows
```

or pass it as a define option with
```
cmake .. -DPYTHON=3.x
```

The library will also be placed in the `build` directory.

### Run

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

The default will run a 1000 games with the default rules, for each possible games group with at least 2 different players (*ALL* mode). The three default players types are "first-legal", "random-naive", and "random".
```
./ceramic-arena
```

It will output a recap table.
```
Mode: All
Played 12/12 (12000/12000)   
Games per group:  1000
Games per player: 16000
Time: 1.276e+03 µs (game), 1.315e+01 µs (step), 2.087e+00 µs (state change)
Average moves per game: 97.0

      player | winrate |  avg  |  std  | move time |moves
-------------+---------+-------+-------+-----------+-----
 first-legal |  28.32% |  20.0 |  11.0 | 8.089e+00 | 25.7
random-naive |   2.36% |   3.6 |   5.7 | 1.328e+01 | 24.2
      random |  44.31% |  24.8 |  13.7 | 1.204e+01 | 22.9
```
- *Time (game)*: average time to process a whole game
- *Time (step)*: average time to process an action (player decision + state change)
- *Time (state change)*: average time to process a state change (time taken by player to make a decision not included)

- *winrate*: average winrate, which should be around 25% since there are 4 players
- *avg*: average score
- *std*: standard deviation of the score
- *move time*: average time (in µs) to make a decision
- *moves*: average move count per game

To see the arguments that can be passed, execute with the `-h` flag
```
./ceramic-arena -h
```