#include "wall.hpp"

#include <assert.h>

Wall::Wall(ushort size) : size(size), placed() {
  placed.assign(size*size, false);
}

Wall::Wall(const Rules& rule) : Wall(rule.tile_types) {}

Wall::Wall(const Wall& wall) : size(wall.size), placed(wall.placed) {}

bool Wall::is_placed_at(ushort x, ushort y) const {
  assert(x < size && y < size);
  return placed[x + y * size];
}

const vector<bool> Wall::get_placed() const {
  return placed;
}

vector<vector<bool>> Wall::get_placed_array() const {
  vector<vector<bool>> result;
  vector<bool>::const_iterator slice_start = placed.begin();
  vector<bool>::const_iterator slice_end;
  for(int i = 0; i < size; i++) {
    slice_end = slice_start + size;
    result.push_back(
      vector<bool>(slice_start, slice_end)
    );
    slice_start = slice_end;
  }
  return result;
}
