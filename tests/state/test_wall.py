import pytest
from ceramic.state import Wall
from ceramic.rules import Rules


RULES = [Rules.MINI, Rules.DEFAULT]

N = Rules.DEFAULT.tile_types


@pytest.mark.parametrize("rules", RULES)
def test_wall_init(rules):
    Wall(rules)


@pytest.mark.parametrize("x,y", [
    (0, 0),
    (0, 2),
    (2, 0),
    (N + 1, 2),
    (2, N + 1),
])
def test_wall_methods_exceptions(x, y):
    with pytest.raises(ValueError):
        Wall(Rules.DEFAULT).get_placed_at(x, y)


@pytest.mark.parametrize("x,y", [
    (1, 1),
    (2, 3),
    (N, N),
])
def test_wall_methods(x, y):
    wall = Wall(Rules.DEFAULT)
    assert not wall.get_placed_at(x, y)


def test_wall_others():
    assert len(Wall(Rules.DEFAULT).get_placed()) == N * N
    assert len(Wall(Rules.DEFAULT).get_tiles()) == N * N
    array = Wall(Rules.DEFAULT).get_placed_array()
    assert len(array) == N
    assert len(array[0]) == N
    array = Wall(Rules.DEFAULT).get_tiles_array()
    assert len(array) == N
    assert len(array[0]) == N
