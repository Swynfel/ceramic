import pytest
from ceramic.rules import Rules
from ceramic.state import Center, Factory, Panel, Pyramid, State, Tile, Tiles, Wall
from ceramic import TILE_TYPES

E = tuple()


@pytest.mark.parametrize("cls, args", [
    (Rules, E),
    (Center, E),
    (Factory, (0,)),
    (Panel, (Rules.DEFAULT,)),
    (Pyramid, (TILE_TYPES,)),
    (State, (Rules.DEFAULT,)),
    (Tile, E),
    (Tile, (0,)),
    (Tiles, E),
    (Wall, (Rules.DEFAULT,)),
])
def test_str_repr(cls, args):
    obj = cls(*args)
    str(obj)
    repr(obj)
