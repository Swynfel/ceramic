import pytest
from ceramic.state import Tiles, Tile
from ceramic import TILE_TYPES

# Tiles

TILES_VALUES = [
    (5, 3, 0),
    (1, 2, 3, 4, 5),
]


def test_tiles_init_others():
    Tiles()
    tiles = Tiles(Tile())
    Tiles(tiles)


@pytest.mark.parametrize("values", TILES_VALUES)
def test_tiles_array_getter(values):
    tiles = Tiles(values)
    for i, value in enumerate(values):
        assert tiles[Tile(i)] == value


@pytest.mark.parametrize("values", TILES_VALUES)
def test_tiles_array_setter(values):
    tiles = Tiles()
    for i, value in enumerate(values):
        tiles[Tile(i)] = value
    assert tiles == Tiles(values)


@pytest.mark.parametrize("left, right, is_le, is_eq, is_ge", [
    ([1, 2, 3], [1, 2, 3], True, True, True),
    ([0, 1, 2], [1, 2, 3], True, False, False),
    ([1, 2, 3, 4, 5], [1, 2, 3, 4, 5], True, True, True),
    ([3, 3, 3, 3, 3], [1, 2, 3, 4, 5], False, False, False),
    ([0, 1, 2, 1, 0], [1, 1, 2, 2, 2], True, False, False),
    ([0, 1, 0, 1, 0], [0, 0, 0, 1, 0], False, False, True),
])
def test_tiles_comparasions(left, right, is_le, is_eq, is_ge):
    left_tiles = Tiles(left)
    right_tiles = Tiles(right)
    assert (left_tiles <= right_tiles) == is_le
    assert (left_tiles == right_tiles) == is_eq
    assert (left_tiles >= right_tiles) == is_ge


# Operations

@pytest.mark.parametrize("left, right", [
    ([1, 2, 3], [0, 0, 2])
])
def test_tiles_operation(left, right):
    total = [l + r for l, r in zip(left, right)]
    left_tiles = Tiles(left)
    right_tiles = Tiles(right)
    total_tiles = Tiles(total)
    assert left_tiles + right_tiles == total_tiles
    assert total_tiles.total() == sum(total)


@pytest.mark.parametrize("tiles", [
    [1, 2, 3],
    [0, 0, 0, 0, 0],
    [0, 3, 5, 5, 7]
])
@pytest.mark.parametrize("tile", [
    0,
    1,
    2
])
def test_tile_tiles_operation(tiles, tile):
    _tiles = Tiles(tiles)
    _tile = Tile(tile)
    _tiles += _tile
    assert _tiles == Tiles(tiles) + _tile
    _tiles -= _tile
    assert _tiles == Tiles(tiles)
    if _tiles[tile] == 0:
        with pytest.raises(ValueError):
            _tiles -= _tile


# Constants

def test_tiles_zero():
    assert Tiles() == Tiles.ZERO


def test_tiles_len():
    assert len(Tiles()) == TILE_TYPES
