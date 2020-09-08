import pytest
from ceramic import Wall, TILE_TYPES

SIZES = [3, 5]


@pytest.mark.parametrize("size", SIZES)
def test_wall_init(size):
    Wall(size)


def test_wall_methods():
    wall = Wall(TILE_TYPES)
    assert not wall.is_placed_at(0, 0)
    assert len(wall.get_placed()) == TILE_TYPES * TILE_TYPES
