import pytest
from ceramic.game import Action
from ceramic.rules import Rules
from ceramic.state import Center, Factory, Panel, Pyramid, State, Tile, Tiles, Wall
from ceramic import TILE_TYPES

E = tuple()


@pytest.mark.parametrize("cls, args", [
    (Action, (0, Tile.NONE, 0)),
    (Rules, E),
    (Center, E),
    (Factory, (0,)),
    (Panel, (Rules.DEFAULT,)),
    (Pyramid, (TILE_TYPES,)),
    (State, (Rules.DEFAULT,)),
    (Tile, (0,)),
    (Tiles, E),
    (Wall, (Rules.DEFAULT,)),
])
def test_str_repr(cls, args):
    obj = cls(*args)
    str(obj)
    repr(obj)
