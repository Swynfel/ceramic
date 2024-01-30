# Ceramic

[![Version](https://img.shields.io/github/v/tag/Swynfel/ceramic?label=version)](https://github.com/Swynfel/ceramic)
![Tests](https://github.com/Swynfel/ceramic/workflows/Tests/badge.svg)
[![License](https://img.shields.io/github/license/Swynfel/ceramic)](https://github.com/Swynfel/ceramic/blob/master/LICENSE)

A python module for playing variations of the board game Azul, implemented in C++.

## Install through pip

The python module can be simply installed through pip using the following command.

```
pip install git+https://github.com/Swynfel/ceramic.git
```

You should now be able to use the module.
For example, to create a player that always choose the Action "take from Factory **1** tiles of color **B** and place them on line **3**" if it is available, or a random action otherwise, you can write:

```python3
from ceramic.game import Action, Player, GameHelper
from ceramic.state import Tile
import random

class TestPlayer(Player):
    def __init__(self):
        Player.__init__(self)

    def play(self, state):
        special_action = Action(1, Tile.from_letter('B'), 3)
        if (GameHelper.legal(special_action, state)):
            return special_action
        legal_actions = GameHelper.all_legal(state)
        return random.choice(legal_actions)
```

To run a game with the base rules, with this newly created agent and three random agents, you can add:

```python3
from ceramic.game import Game
from ceramic.rules import Rules
from ceramic.players import RandomPlayer

game = Game(Rules.BASE)
game.add_player(TestPlayer())
game.add_players([RandomPlayer() for i in range(0,3)])
game.roll_game() # Plays a random game until the end
print("The winner is:", game.state.winning_player())
```

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
pip install pybind11
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
============================ 137 passed in 0.23s =============================
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

#### ceramic-terminal-player

Play a game using the terminal.

To play a default game against 3 random players, call

```
./ceramic-terminal-player
```

To play against one random player and one first-legal player, using colors in the terminal, call

```
./ceramic-terminal-player r fl -c f
```

To play against 3 monte-carlo with 10000 complete roll-outs (the best AI available), call

```
./ceramic-terminal-player 'mc{10000,u:f}' -p 3 -c f
```

For additional options, call with `-h` option.

```
./ceramic-terminal-player -h
```

You can type `-help` during the game to see action format.

#### ceramic-arena

Compare the performance of different agents.

The default will run a 1000 games with the default rules, for each possible games group with at least 2 different players (_ALL_ mode). The three default players types are "first-legal", "random-naive", and "random".

```
./ceramic-arena
```

It will output a recap table.

```
Mode: All
Played 12/12 (12000/12000)
Games per group:  1000
Games per player: 16000
Total time: 9.0917e+05 µs (real), 7.2734e+06 (times thread count)
Time: 5.240e+02 µs (game), 5.404e+00 µs (step), 1.032e+00 µs (state change)
Average moves per game: 97.0

      player | winrate |  avg  |  std  | move time |moves
-------------+---------+-------+-------+-----------+-----
 first-legal |  27.06% |  19.8 |  11.0 | 3.787e+00 | 25.6
random-naive |   2.42% |   3.6 |   5.7 | 4.969e+00 | 24.2
      random |  45.52% |  25.2 |  13.8 | 4.396e+00 | 23.0
```

- _Total time (real)_: the total time elapse
- _Total time (times thread count)_: above duration multiplied by number of threads
- _Time (game)_: average time to process a whole game
- _Time (step)_: average time to process an action (player decision + state change)
- _Time (state change)_: average time to process a state change (time taken by player to make a decision not included)

- _winrate_: average winrate, which should be around 25% since there are 4 players
- _avg_: average score
- _std_: standard deviation of the score
- _move time_: average time (in µs) to make a decision
- _moves_: average move count per game

To see the arguments that can be passed, execute with the `-h` flag

```
./ceramic-arena -h
```

## Citation

The environment was presented in a workshop, the article can be found here: http://id.nii.ac.jp/1001/00207567/

```
@inproceedings{gendre2020ceramic,
    author = {Quentin Gendre and Tomoyuki Kaneko},
    title = {Ceramic: A research environment based on the multi-player strategic board game Azul},
    booktitle = {ゲームプログラミングワークショップ 2020 論文集},
    year = {2020},
    volume = {2020},
    pages = {155--160},
    month = {nov},
    url = {http://id.nii.ac.jp/1001/00207567/}
}
```
