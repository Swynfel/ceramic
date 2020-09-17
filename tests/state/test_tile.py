import pytest
from ceramic.state import Tile
from ceramic import TILE_TYPES


TILE_VALUES = [0, 1, 2, 3, 4]


@pytest.mark.parametrize("value", TILE_VALUES)
def test_tile(value):
    tile = Tile(value)
    assert value == int(tile)
    assert tile == Tile(value)


def test_tile_init_others():
    assert Tile(Tile.NONE) == Tile.NONE
    with pytest.raises(ValueError):
        Tile(TILE_TYPES + 1)
