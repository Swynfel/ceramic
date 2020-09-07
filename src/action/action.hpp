#ifndef ACTION_HPP
#define ACTION_HPP

#include "../global.hpp"
#include "../state/tiles.hpp"

struct Action {
  ushort pick; // id of Factory to pick
  Tile color; // color to take in Factory
  ushort place; // line on Pyramid to place

  Action(ushort pick, Tile color, ushort place);
};

#endif //ACTION_HPP
