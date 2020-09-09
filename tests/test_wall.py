import pytest
from ceramic import Wall, TILE_TYPES

SIZES = [3, 5]


@pytest.mark.parametrize("size", SIZES)
def test_wall_init(size):
    Wall(size)


@pytest.mark.parametrize("x,y", [
    (0, 0),
    (0, 2),
    (2, 0),
    (TILE_TYPES + 1, 2),
    (2, TILE_TYPES + 1),
])
def test_wall_methods_exceptions(x, y):
    with pytest.raises(ValueError):
        Wall(TILE_TYPES).is_placed_at(x, y)


@pytest.mark.parametrize("x,y", [
    (1, 1),
    (2, 3),
    (TILE_TYPES, TILE_TYPES),
])
def test_wall_methods(x, y):
    wall = Wall(TILE_TYPES)
    assert not wall.is_placed_at(x, y)


def test_wall_others():
    assert len(Wall(TILE_TYPES).get_placed()) == TILE_TYPES * TILE_TYPES
