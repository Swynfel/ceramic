#include "observer.hpp"

#include "game.hpp"

void
Observer::observe_game(Game* game) {
    observed_game = game;
}

void
Observer::delete_game() {
    observed_game = nullptr;
}

Observer::~Observer() {
    if (observed_game != nullptr) {
        observed_game->remove_observer(this);
    }
}