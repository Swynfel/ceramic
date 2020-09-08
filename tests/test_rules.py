# import pytest
from ceramic import Rules


def test_rules_set():
    rules = Rules()
    rules.player_count = 2
    rules.tile_count = 80
    rules.tile_types = 4
    rules.line_bonus = 3
    rules.column_bonus = 3
    rules.type_bonus = 3
    rules.overflow_count = 5
    rules.overflow_penalty = 5


def test_rules_methods():
    assert Rules.DEFAULT.tile_types == 5
