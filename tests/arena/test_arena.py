import pytest
from ceramic.players import RandomPlayer
from ceramic.arena import Arena, AllArena, PairsArena
from ceramic.rules import Rules


@pytest.mark.parametrize("arena_class", [Arena, AllArena])
@pytest.mark.parametrize("rules", [Rules.MINI, Rules.BASE])
def test_multi_arenas(arena_class, rules):
    players = [RandomPlayer() for p in range(0, rules.player_count+1)]
    arena = arena_class(rules, players)
    arena.count = 1
    arena.run()
    arena.print()


@pytest.mark.parametrize("rules", [Rules.MINI, Rules.BASE])
def test_pair_arenas(rules):
    players = [RandomPlayer(), RandomPlayer()]
    arena = PairsArena(rules, players)
    arena.count = 1
    arena.run()
    arena.print()
