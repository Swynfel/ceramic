#include "terminal_player.hpp"

#include "game/game.hpp"

#include <iostream>
#include <sstream>

void
TerminalPlayer::help() const {
    std::cout << '\n';
    std::cout << "Help\n";
    std::cout << '\n';
    std::cout << "Enter options with an '-' suffix\n";
    std::cout << "    -full, -grey or -none to change color\n";
    std::cout << "    -state to display state again\n";
    std::cout << "    -actions to display legal actions, type ''\n";
    std::cout << '\n';
    std::cout << "Enter Action with three characters: xyz\n";
    std::cout << "    x (int) is picked Center (0) or Factory\n";
    std::cout << "    y (letter) is tile color\n";
    std::cout << "    z (int) is placed Floor (0) or Pyramid Line\n";
    std::cout << "Examples:\n";
    std::cout << "    2a4 will take color 'a' from Factory#2 and place it on line 4\n";
    std::cout << "    0b1 will take color 'b' from Center and place it on line 1\n";
    std::cout << "    1c0 will take color 'c' from Factory#1 and throw it in floor" << std::endl;
}

string
TerminalPlayer::colored_token(string token) const {
    if (color_type == TerminalPlayer::COLORED_TYPE::FULL) {
        return "<\x1b[30;47m" + token + "\x1b[0m>";
    } else {
        return "<" + token + ">";
    }
}

string
TerminalPlayer::colored_tile(Tile tile, bool colored) const {
    if (color_type == TerminalPlayer::COLORED_TYPE::NONE) {
        return string(1, colored ? tile.letter() : tile.lc_letter());
    }
    if (color_type == TerminalPlayer::COLORED_TYPE::GREY) {
        if (colored) {
            return string(1, tile.letter());
        }
        return "\x1b[30m" + string(1, tile.lc_letter()) + "\x1b[0m";
    }
    ostringstream os;
    os << "\x1b[";
    if (colored && tile != Tile::NONE) {
        os << "7;";
    }
    if (tile == Tile::NONE) {
        os << "100";
    } else {
        os << "3" << (int(tile) + 1);
    }
    os << "m";
    if (colored) {
        os << tile.letter();
    } else {
        os << tile.lc_letter();
    }
    os << "\x1b[0m";
    return os.str();
}

string
TerminalPlayer::colored_tiles(Tiles tiles) const {
    ostringstream os;
    for (ushort tile_value = 0; tile_value < TILE_TYPES; tile_value++) {
        Tile tile = Tile(tile_value);
        for (int i = 0; i < tiles[tile]; i++) {
            os << colored_tile(tile);
        }
    }
    return os.str();
}

void
TerminalPlayer::print_state(const State& state) const {
    const std::shared_ptr<const Rules>& rules = state.get_rules();
    bool brackets = color_type != TerminalPlayer::COLORED_TYPE::FULL;
    // Center
    std::cout << "Center#0:" << colored_tiles(state.get_center());
    if (state.get_center().first_token) {
        std::cout << colored_token("+");
    }
    std::cout << '\n';
    // Factories
    for (int i = 1; i <= rules->factory_count(); i++) {
        const Factory& factory = state.get_factory(i);
        std::cout << "Factory#" << i << ":" << colored_tiles(factory) << '\n';
    }
    // Players
    for (int player = 0; player < rules->player_count; player++) {
        std::cout << "------------------\n";
        bool current_player = player == state.get_current_player();
        const Panel& panel = state.get_panel(player);
        std::cout << "Score: " << panel.get_score();
        if (current_player) {
            std::cout << " " << colored_token("current");
        }
        if (panel.get_first_token()) {
            std::cout << " " << colored_token("+");
        }
        std::cout << '\n';
        for (int line = 1; line <= rules->tile_types; line++) {
            const Wall& wall = panel.get_wall();
            if (brackets) {
                std::cout << "[";
            }
            for (int i = 1; i <= rules->tile_types; i++) {
                Tile color = wall.color_at(i, line);
                std::cout << colored_tile(color, wall.get_placed_at(i, line));
            }
            if (brackets) {
                std::cout << "]";
            }
            std::cout << ' ';
            const Pyramid& pyramid = panel.get_pyramid();
            int a = pyramid.amount(line);
            Tile tile = pyramid.color(line);
            if (brackets) {
                std::cout << "[";
            }
            for (int i = 1; i <= line; i++) {
                std::cout << colored_tile(i <= a ? tile : Tile::NONE);
            }
            if (brackets) {
                std::cout << "]";
            }
            if (current_player) {
                std::cout << " <-- " << line;
            }
            std::cout << '\n';
        }
        std::cout << "Floor: " << panel.get_floor() << " (-" << panel.get_penalty() << ")";
        if (current_player) {
            std::cout << " <-- 0";
        }
        std::cout << std::endl;
    }
}

void
TerminalPlayer::options(const State& state) {
    string opt;
    std::getline(std::cin, opt);
    if (opt == "-full") {
        color_type = TerminalPlayer::COLORED_TYPE::FULL;
        return;
    }
    if (opt == "-grey") {
        color_type = TerminalPlayer::COLORED_TYPE::GREY;
        return;
    }
    if (opt == "-none") {
        color_type = TerminalPlayer::COLORED_TYPE::NONE;
        return;
    }
    if (opt == "-state") {
        print_state(state);
        return;
    }
    if (opt == "-actions") {
        vector<Action> legal = Game::all_legal(state);
        for (Action a : legal) {
            std::cout << a << " ";
        }
        std::cout << std::endl;
        return;
    }
    if (opt == "-help") {
        help();
        return;
    }
    throw invalid_argument("Invalid command");
}

// Public
void
TerminalPlayer::set_color_type(TerminalPlayer::COLORED_TYPE _color_type) {
    color_type = _color_type;
}

Action
TerminalPlayer::play(const State& state) {
    std::cout << '\n';
    if (successful) {
        print_state(state);
    }
    Action action;
    while (std::cin) {
        try {
            std::cout << "Action> " << std::flush;
            switch (std::cin.peek()) {
                case EOF:
                    break;
                case '-':
                    options(state);
                    break;
                default:
                    std::cin >> action;
                    return action;
            }
        } catch (const std::invalid_argument& e) {
            std::cout << std::flush;
            std::cout << e.what() << '\n'
                      << "Type '-help' for help\n"
                      << std::endl;
        }
    }
    throw runtime_error("Exit program");
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


std::shared_ptr<Observer>
TerminalPlayer::observer() {
    return shared_from_this();
}

void
TerminalPlayer::end_game(const State& state, ushort winner_position) {
    std::cout << '\n';
    std::cout << "+---------------+\n";
    std::cout << "| Game finished |\n";
    std::cout << "+---------------+\n";
    std::cout << '\n';
    print_state(state);
    std::cout << '\n';
    std::cout << "Winner: " << state.winning_player() << '\n';
    std::cout << "Scores:";
    for (ushort p = 0U; p < state.get_rules()->player_count; p++) {
        std::cout << " " << state.get_panel(p).get_score();
    }
    std::cout << std::endl;
}