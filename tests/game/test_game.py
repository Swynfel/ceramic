import random
import pytest
from ceramic.game import Game, Action, Player, GameHelper
from ceramic.players import FirstLegalPlayer, RandomPlayer, MonteCarloPlayer
from ceramic.state import Tile, Tiles
from ceramic.rules import Rules


@pytest.mark.parametrize("rules", [Rules.MINI, Rules.DEFAULT])
def test_game_methods(rules):
    game = Game(rules, 0)
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
@pytest.mark.parametrize("player", [
    FirstLegalPlayer(),
    RandomPlayer(smart=False),
    RandomPlayer(),
    MonteCarloPlayer(rollouts=30)
])
def test_game_manual_roll(rules, player):
    game = Game(rules, 0)
    expected_total_tiles = Tiles([rules.tile_count] * rules.tile_types)
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
    class PythonRandomPlayer(Player):
        def __init__(self):
            Player.__init__(self)

        def play(self, state):
            possible_actions = GameHelper.all_smart_legal(state)
            if len(possible_actions) == 0:
                possible_actions = GameHelper.all_penalty_legal(state)
            return random.choice(possible_actions)

    game = Game(rules)
    python_random_player = PythonRandomPlayer()
    game.add_players(
        [python_random_player for _ in range(0, rules.player_count)])
    game.roll_game()
    assert max([game.state.panel(p).wall.completed_line_count()
                for p in range(0, rules.player_count)]) > 0
