#ifndef TERMINAL_PLAYER
#define TERMINAL_PLAYER

#include "game/action.hpp"
#include "game/player.hpp"
#include "global.hpp"
#include "state/state.hpp"

class TerminalPlayer
  : public std::enable_shared_from_this<TerminalPlayer>
  , public Player
  , public Observer {
public:
    enum ColoredType {
        NONE,
        GREY,
        FULL
    };

private:
    TerminalPlayer::ColoredType color_type = TerminalPlayer::ColoredType::GREY;
    bool successful = true;

    void help() const;

    std::string colored_token(std::string token) const;
    std::string colored_tile(Tile tile, bool colored = true) const;
    std::string colored_tiles(Tiles tiles) const;
    void print_state(const State& state) const;

    void options(const State& state);

public:
    void set_color_type(TerminalPlayer::ColoredType color_type);

    Action play(const State& state) override;

    void error(std::string error) override;

    std::string player_type() const override;

    std::shared_ptr<Observer> observer() override;
    void end_game(const State& state, ushort winner_position) override;
};

#endif //TERMINAL_PLAYER
