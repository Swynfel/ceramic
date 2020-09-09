# import pytest
from ceramic.state import Factory, Tiles


def test_factory_fields():
    factory = Factory(0)
    isinstance(factory.id, int)
    isinstance(factory.tiles, Tiles)
