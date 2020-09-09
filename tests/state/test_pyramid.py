import pytest
from ceramic.rules import Rules
from ceramic.state import Pyramid, Tile
from ceramic import TILE_TYPES

RULES = [Rules.MINI, Rules.DEFAULT]
SIZES = [3, 5]


@pytest.mark.parametrize("arg", SIZES + RULES)
def test_pyramid_init(arg):
    Pyramid(arg)


def test_pyramid_methods():
    pyramid = Pyramid(TILE_TYPES)
    assert pyramid.is_empty(2)
    assert not pyramid.is_filled(2)
    assert pyramid.color(2) == Tile.NONE

    pyramid.set_line(2, 1, Tile(2))
    assert not pyramid.is_empty(2)
    assert not pyramid.is_filled(2)
    assert pyramid.color(2) == Tile(2)

    with pytest.raises(ValueError):
        pyramid.set_line(2, 2, Tile(1))
    pyramid.set_line(2, 2, Tile(2))
    assert not pyramid.is_empty(2)
    assert pyramid.is_filled(2)
    assert pyramid.color(2) == Tile(2)
