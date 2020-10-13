import pytest
from ceramic.game import Action
from ceramic.state import Tile
from ceramic.rules import Rules


@pytest.mark.parametrize("pick", [0, 1, Rules.BASE.factory_count])
@pytest.mark.parametrize("color", [Tile(0), Tile(Rules.BASE.tile_types-1)])
@pytest.mark.parametrize("place", [0, 1, Rules.BASE.tile_types])
def test_action_fields(pick, color, place):
    action1 = Action(pick, color, place)
    action2 = Action(pick=pick, color=color, place=place)
    assert action1 == action2
    assert action1.pick == pick
    assert action1.color == color
    assert action1.place == place
