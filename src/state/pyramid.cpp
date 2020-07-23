#include "pyramid.hpp"

Pyramid::Pyramid (ushort size) : size(size), tile_types(), tile_filled() {
  tile_types.assign(size, Tile::NONE);
  tile_filled.assign(size, 0);
}

Pyramid::Pyramid (const Rules& rule) : Pyramid(rule.tile_types) {}

Pyramid::Pyramid (const Pyramid& pyramid) : size(pyramid.size), tile_types(pyramid.tile_types), tile_filled(pyramid.tile_filled) {}

bool Pyramid::is_filled(ushort line) const {
  return tile_filled[line] == line;
}

vector<bool> Pyramid::filled() const {
  vector<bool> result;
  for(ushort i = 0; i < size; i++) {
    result.push_back(is_filled(i));
  }
  return result;
}
