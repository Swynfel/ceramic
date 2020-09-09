#import pytest
from ceramic.state import Center, Tiles


def test_center_fields():
    center = Center()
    isinstance(center.first_token, bool)
    isinstance(center.tiles, Tiles)
