#include "tiles.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>
#include <sstream>
#include <iostream>
#include <assert.h>

#include <stdexcept>


/*** Tile ***/

Tile::Tile(): value(TILE_TYPES) {}

Tile::Tile(ushort value): value(value) {
  if(value >= TILE_TYPES) {
    throw std::invalid_argument("Tile color was too big");
  }
}

Tile::Tile(const Tile& tile): value(tile.value) {}

const Tile Tile::NONE = Tile();


Tile::operator int() const {
  return value;
}


ostream& operator<<(ostream& os, const Tile& tile) {
  if (tile.is_none()) {
    return os << "< >";
  }
  return os << '<' << (int)tile << '>';
}

bool operator==(const Tile& left, const Tile& right) {
  return left.value == right.value;
}

bool operator!=(const Tile& left, const Tile& right) {
  return !(left == right);
}

bool Tile::is_none() const {
  return (this->value == TILE_TYPES);
}

string Tile::str() const {
  ostringstream os;
  os << *this;
  return os.str();
}


/*** Tiles ***/

// Constructors

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

Tiles::Tiles(const Tiles& tiles) : quantities(tiles.quantities) {}

// Utils

ushort& Tiles::operator[](Tile tile) {
  return quantities[tile];
}

ushort Tiles::total() const {
  return accumulate(
    this->quantities.begin(),
    this->quantities.end(),
    0
  );
}

Tiles& Tiles::operator+=(const Tiles& other) {
  for(int i = 0; i<TILE_TYPES; i++){
    this->quantities[i] += other.quantities[i];
  }
  return *this;
}

Tiles& Tiles::operator-=(const Tiles& other) {
  assert(*this >= other);
  for(int i = 0; i<TILE_TYPES; i++){
    this->quantities[i] -= other.quantities[i];
  }
  return *this;
}

Tiles Tiles::operator+(const Tiles& other) const {
  Tiles t(*this);
  t += other;
  return t;
}

Tiles Tiles::operator-(const Tiles& other) const {
  Tiles t(*this);
  t -= other;
  return t;
}

bool operator==(const Tiles& left, const Tiles& right) {
  return left.quantities == right.quantities;
}

bool operator!=(const Tiles& left, const Tiles& right) {
  return !(left == right);
}

bool operator<=(const Tiles& left, const Tiles& right) {
  for(int i = 0; i<TILE_TYPES; i++){
    if(left.quantities[i] > right.quantities[i]) {
      return false;
    }
  }
  return true;
}

bool operator>=(const Tiles& left, const Tiles& right) {
  for(int i = 0; i<TILE_TYPES; i++){
    if(left.quantities[i] < right.quantities[i]) {
      return false;
    }
  }
  return true;
}


Tiles& Tiles::operator+=(const Tile& tile) {
  this->quantities[tile]++;
  return *this;
}

Tiles& Tiles::operator-=(const Tile& tile) {
  assert(this->quantities[tile] > 0);
  this->quantities[tile]--;
  return *this;
}

Tiles Tiles::operator+(const Tile& tile) const {
  Tiles t(*this);
  t += tile;
  return t;
}

Tiles Tiles::operator-(const Tile& tile) const {
  Tiles t(*this);
  t -= tile;
  return t;
}


bool operator<=(const Tiles& left, const ushort& right) {
  for(int i = 0; i<TILE_TYPES; i++){
    if(left.quantities[i] > right) {
      return false;
    }
  }
  return true;
}

bool operator>=(const Tiles& left, const ushort& right) {
  for(int i = 0; i<TILE_TYPES; i++){
    if(left.quantities[i] < right) {
      return false;
    }
  }
  return true;
}


// Reading

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
