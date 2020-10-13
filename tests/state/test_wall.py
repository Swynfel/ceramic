import pytest
from ceramic.state import Wall
from ceramic.rules import Rules


RULES = [Rules.MINI, Rules.BASE]

N = Rules.BASE.tile_types


@pytest.mark.parametrize("rules", RULES)
def test_wall_init(rules):
    Wall(rules)


@pytest.mark.parametrize("x,y", [
    (0, 0),
    (0, 2),
    (2, 0),
    (N + 1, 2),
    (2, N + 1),
])
def test_wall_methods_exceptions(x, y):
    with pytest.raises(ValueError):
        Wall(Rules.BASE).get_placed_at(x, y)


@pytest.mark.parametrize("x,y", [
    (1, 1),
    (2, 3),
    (N, N),
])
def test_wall_methods(x, y):
    wall = Wall(Rules.BASE)
    assert not wall.get_placed_at(x, y)


def test_wall_others():
    assert len(Wall(Rules.BASE).get_placed()) == N * N
    assert len(Wall(Rules.BASE).get_tiles()) == N * N
    array = Wall(Rules.BASE).get_placed_array()
    assert len(array) == N
    assert len(array[0]) == N
    array = Wall(Rules.BASE).get_tiles_array()
    assert len(array) == N
    assert len(array[0]) == N


@pytest.mark.parametrize("x", [1, 2, 3, 4, 5])
@pytest.mark.parametrize("y", [1, 2, 3, 4, 5])
def test_wall_layout(x, y):
    wall_manual = Wall(Rules.BASE)
    wall_color = Wall(Rules.BASE)
    walls = [wall_manual, wall_color]
    c = wall_manual.color_at(x, y)
    assert c == wall_color.color_at(x, y)
    for wall in walls:
        assert not wall.get_placed_at(x, y)
        assert not wall.get_tile_at(x, y)
        assert not wall.line_has_color(y, c)
    wall_manual.place_at(x, y)
    wall_color.place_line_color(y, c)
    for wall in walls:
        assert wall.get_placed_at(x, y)
        assert wall.get_tile_at(x, y) == c
        assert wall.line_has_color(y, c)
