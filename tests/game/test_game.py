import pytest
from ceramic.game import Game, Action, Player, legal
from ceramic.players import FirstLegalPlayer
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
    player = FirstLegalPlayer()
    while not game.state.is_game_finished():
        game.start_round()
        while not game.state.is_round_finished():
            assert game.state.get_total_tiles() == expected_total_tiles
            action = player.play(game.state)
            game.apply(action)
            game.next_player()
        game.end_round()
    assert max([game.state.panel(p).wall.completed_line_count()
                for p in range(0, rules.player_count)]) > 0


@pytest.mark.parametrize("rules", [Rules.MINI, Rules.DEFAULT])
def test_game_roll_with_python_player(rules):
    class PythonFirstLegalPlayer(Player):
        def __init__(self):
            Player.__init__(self)

        def play(self, state):
            rules = state.rules
            for place in range(rules.tile_types, -1, -1):
                for pick in range(0, rules.factory_count + 1):
                    for color in range(0, rules.tile_types):
                        action = Action(
                            pick=pick, color=Tile(color), place=place)
                        if(legal(action, state)):
                            return action
            raise RuntimeError("No legal action")

    game = Game(rules)
    players = [PythonFirstLegalPlayer() for _ in range(0, rules.player_count)]
    game.add_players(players)
    game.roll_game()
    assert max([game.state.panel(p).wall.completed_line_count()
                for p in range(0, rules.player_count)]) > 0
