import pytest
from ceramic import Pyramid, Tile, TILE_TYPES

SIZES = [3, 5]


@pytest.mark.parametrize("size", SIZES)
def test_pyramid_init(size):
    Pyramid(size)


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
    pyramid.set_line(2, 3, Tile(2))
    assert not pyramid.is_empty(2)
    assert pyramid.is_filled(2)
    assert pyramid.color(2) == Tile(2)
