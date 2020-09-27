import pytest
from ceramic.rules import Rules


@pytest.mark.parametrize("player_count", [2, 4])
@pytest.mark.parametrize("tile_count,tile_types,factory_tiles", [
    (15, 3, 3),
    (20, 5, 4),
])
@pytest.mark.parametrize("line_bonus,column_bonus,type_bonus", [
    (3, 2, 5),
    (7, 3, 15),
])
@pytest.mark.parametrize("overflow_count,overflow_penalty", [
    (4, 2),
    (7, 3),
])
def test_rules_init(
        player_count,
        tile_count,
        tile_types,
        factory_tiles,
        line_bonus,
        column_bonus,
        type_bonus,
        overflow_count,
        overflow_penalty):
    # pylint: disable=too-many-arguments
    rules = Rules()
    rules.player_count = player_count
    rules.tile_count = tile_count
    rules.tile_types = tile_types
    rules.factory_tiles = factory_tiles
    rules.line_bonus = line_bonus
    rules.column_bonus = column_bonus
    rules.type_bonus = type_bonus
    rules.overflow_count = overflow_count
    rules.overflow_penalty = overflow_penalty
    assert rules == Rules(
        player_count=player_count,
        tile_count=tile_count,
        tile_types=tile_types,
        factory_tiles=factory_tiles,
        line_bonus=line_bonus,
        column_bonus=column_bonus,
        type_bonus=type_bonus,
        overflow_count=overflow_count,
        overflow_penalty=overflow_penalty,
    )


def test_rules_methods():
    assert Rules.DEFAULT.tile_types == 5
