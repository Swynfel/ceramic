import pytest
from ceramic.rules import Rules
from ceramic.state import Pyramid, Tile
from ceramic import TILE_TYPES

RULES = [Rules.MINI, Rules.BASE]
SIZES = [3, 5]


@pytest.mark.parametrize("arg", SIZES + RULES)
def test_pyramid_init(arg):
    Pyramid(arg)


def test_pyramid_methods():
    pyramid = Pyramid(TILE_TYPES)
    line = 2
    tile1 = Tile(1)
    tile2 = Tile(2)

    assert pyramid.accept_color(line, tile1)
    assert pyramid.accept_color(line, tile2)
    assert pyramid.is_empty(line)
    assert not pyramid.is_filled(line)
    assert pyramid.color(line) == Tile.NONE

    pyramid.set_line(line, 1, tile1)
    assert pyramid.accept_color(line, tile1)
    assert not pyramid.accept_color(line, tile2)
    assert not pyramid.is_empty(line)
    assert not pyramid.is_filled(line)
    assert pyramid.color(line) == tile1

    pyramid.set_line(line, line, tile2)
    assert not pyramid.accept_color(line, tile1)
    assert not pyramid.accept_color(line, tile2)
    assert not pyramid.is_empty(line)
    assert pyramid.is_filled(line)
    assert pyramid.color(line) == tile2
