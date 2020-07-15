#include <iostream>

#include "tiles.hpp"

int main() {
  Tile tile{3};
  Tiles tiles(tile);
  std::cout << tiles;
  return 0;
}
