import pytest
from ceramic import Panel, Pyramid, Wall, TILE_TYPES

SIZES = [3, 5]


@pytest.mark.parametrize("size", SIZES)
def test_panel_init(size):
    Panel(size)


def test_panel_fields():
    panel = Panel(TILE_TYPES)
    isinstance(panel.score, int)
    isinstance(panel.pyramid, Pyramid)
    isinstance(panel.wall, Wall)
    isinstance(panel.first_token, bool)
    isinstance(panel.floor, int)
