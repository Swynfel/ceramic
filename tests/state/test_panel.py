import pytest
from ceramic.rules import Rules
from ceramic.state import Panel, Pyramid, Wall

RULES = [Rules.MINI, Rules.BASE]


@pytest.mark.parametrize("rules", RULES)
def test_panel_init(rules):
    Panel(rules)


def test_panel_fields():
    panel = Panel(Rules.BASE)
    isinstance(panel.score, int)
    isinstance(panel.pyramid, Pyramid)
    isinstance(panel.wall, Wall)
    isinstance(panel.first_token, bool)
    isinstance(panel.floor, int)
