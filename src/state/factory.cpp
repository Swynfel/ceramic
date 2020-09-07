#include "factory.hpp"

// Constructor

Factory::Factory(ushort _id)
  : id(_id)
  , tiles() {}

// Utils

Tiles
Factory::get_tiles() const {
    return tiles;
}
