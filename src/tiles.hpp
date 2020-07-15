#ifndef TILES_H
#define TILES_H

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "global.hpp"

using namespace std;

struct Tile {
private:
  ushort value;
public:
  Tile(ushort _value);

  operator int() const;
};

class Tiles {
  array<ushort, TILE_TYPES> quantities;
public:
  Tiles();
  Tiles(const Tile& tile);
  Tiles(const vector<ushort>& tiles);
  Tiles(const Tiles& tiles);

  ushort total() const;
  ushort& operator[](Tile tile);
  friend ostream& operator <<(ostream& os, const Tiles& tiles);
  string str() const;
  array<ushort, TILE_TYPES> to_tuple() const;
};

#endif //TILES_H
