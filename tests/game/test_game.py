import pytest
from ceramic.game import Game, Action, Player
from ceramic.state import Tile, Tiles
from ceramic.rules import Rules


@pytest.mark.parametrize("rules", [Rules.MINI, Rules.DEFAULT])
def test_game_methods(rules):
    game = Game(rules)
    game.reset()
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
    expected_total_tiles = Tiles([rules.tile_count] * rules.tile_types)
    player = Player()
    while not game.state.is_game_finished():
        game.start_round()
        while not game.state.is_round_finished():
            assert game.state.get_total_tiles() == expected_total_tiles
            action = player.play(game.state)
            game.apply(action)
            game.next_player()
        game.end_round()
