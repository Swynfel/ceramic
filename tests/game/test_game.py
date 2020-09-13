import pytest
from ceramic.game import Game, Action, Player
from ceramic.state import Tile
from ceramic.rules import Rules


@pytest.mark.parametrize("rules", [Rules.MINI, Rules.DEFAULT])
def test_game_methods(rules):
    game = Game(rules)
    game.start()
    game.start_round()
    game.end_round()
    game.setup_factories()
    game.score_panels()
    game.apply_first_token()
    # Action has to be invalid because center is empty
    assert not game.legal(Action(0, Tile(0), 0))
    with pytest.raises(ValueError):
        game.apply(Action(0, Tile(0), 0))


@pytest.mark.parametrize("rules", [Rules.MINI, Rules.DEFAULT])
def test_game_roll(rules):
    game = Game(rules)
    for _ in range(0, rules.player_count):
        game.add_player(Player())
    game.roll_game()
