import pytest
from ceramic import Factory, Panel, Pyramid, Rules, State, Tile, Tiles, Wall, TILE_TYPES

E = tuple()


@pytest.mark.parametrize("cls, args", [
    (Factory, (TILE_TYPES,)),
    (Panel, (TILE_TYPES,)),
    (Pyramid, (TILE_TYPES,)),
    (Rules, E),
    (State, (Rules.DEFAULT,)),
    (Tile, E),
    (Tile, (0,)),
    (Tiles, E),
    (Wall, (TILE_TYPES,)),
])
def test_str_repr(cls, args):
    obj = cls(*args)
    str(obj)
    repr(obj)
