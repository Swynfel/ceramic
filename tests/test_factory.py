import pytest
from ceramic import Factory, Tiles

SIZES = [3, 5]


@pytest.mark.parametrize("size", SIZES)
def test_factory_init(size):
    Factory(size)


def test_factory_fields():
    isinstance(Factory(0).tiles, Tiles)
