#include "terminal_player.hpp"

#include <iostream>

void
TerminalPlayer::help() const {
    std::cout << "Enter Action with three characters: xyz" << std::endl;
    std::cout << "    x (int) is picked Center (0) or Factory" << std::endl;
    std::cout << "    y (letter) is tile color" << std::endl;
    std::cout << "    z (int) is placed Floor (0) or Pyramid Line" << std::endl;
}

Action
TerminalPlayer::play(const State& state) {
    std::cout << state;
    std::cout << "Action> ";
    Action action;
    while (true) {
        try {
            std::cin >> action;
            return action;
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            help();
        }
    }
}

void
TerminalPlayer::error(string message) {
    Player::error(message);
    help();
}

string
TerminalPlayer::player_type() const {
    return "terminal-player";
}