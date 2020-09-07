#include "game.hpp"


Game::Game(const std::shared_ptr<Rules>& rules) :
  randomness(),
  state(rules) {}

Game::Game() : Game(Rules::get_default()) {}

Game::Game(const Rules& rules) : Game(std::make_shared<Rules>(rules)) {}
