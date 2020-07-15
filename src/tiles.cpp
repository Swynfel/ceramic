#include "tiles.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>
#include <sstream>
#include <iostream>

// Tile

Tile::Tile(ushort _value): value(_value) {

}

Tile::operator int() const {
  return value;
}

// Tiles

Tiles::Tiles() : quantities() {}

Tiles::Tiles(const Tile& tile) : quantities() {
  quantities[(int)(tile)] = 1;
}

Tiles::Tiles(const vector<ushort>& tiles) : quantities() {
  uint index = 0;
  for(ushort q: tiles) {
    if(index > TILE_TYPES) {
      break;
    }
    quantities[index] = q;
    index++;
  }
}

Tiles::Tiles(const Tiles& tiles) : quantities(tiles.quantities) {
}

ushort Tiles::total() const {
  return accumulate(
    this->quantities.begin(),
    this->quantities.end(),
    0
  );
}

ushort& Tiles::operator[](Tile tile) {
  return quantities[tile];
}

ostream& operator<<(ostream& os, const Tiles& tiles) {
  bool first = true;
  for(uint v : tiles.quantities) {
    if(first){
      first = false;
      os << '[';
    } else {
      os << ',';
    }
    os << v;
  }
  if(first) {
    os << '[';
  }
  os << ']';
  return os;
}

string Tiles::str() const {
  ostringstream os;
  os << *this;
  return os.str();
}

array<ushort, TILE_TYPES> Tiles::to_tuple() const {
  return quantities;
}
