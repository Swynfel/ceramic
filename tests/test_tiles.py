import pytest
from ceramic import Tiles, Tile, TILE_TYPES

# Tile

TILE_VALUES = [0, 1, 2, 3, 4]


@pytest.mark.parametrize("value", TILE_VALUES)
def test_tile(value):
    tile = Tile(value)
    assert value == int(tile)
    assert tile == Tile(value)


def test_tile_init_others():
    assert Tile() == Tile.NONE
    assert Tile(Tile.NONE) == Tile.NONE
    with pytest.raises(ValueError):
        Tile(TILE_TYPES + 1)


@pytest.mark.parametrize("value", TILE_VALUES)
def test_tile_str(value):
    assert str(Tile(value)) == f"<{value}>"


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


@pytest.mark.parametrize("values", TILES_VALUES)
def test_tiles_str(values):
    assert str(Tiles(values)) == \
        str([0 if k >= len(values) else values[k]
             for k in range(0, 5)]).replace(' ', '')


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
    [3, 3, 5, 5, 7]
])
@pytest.mark.parametrize("tile", [
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


# Constants

def test_tiles_zero():
    assert Tiles() == Tiles.ZERO


def test_tiles_len():
    assert len(Tiles()) == TILE_TYPES
