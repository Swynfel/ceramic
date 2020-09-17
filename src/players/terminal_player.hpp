#ifndef TERMINAL_PLAYER
#define TERMINAL_PLAYER

#include "game/action.hpp"
#include "game/player.hpp"
#include "global.hpp"
#include "state/state.hpp"

class TerminalPlayer
  : public enable_shared_from_this<TerminalPlayer>
  , public Player
  , public Observer {
public:
    enum COLORED_TYPE {
        NONE,
        GREY,
        FULL
    };

private:
    TerminalPlayer::COLORED_TYPE color_type = TerminalPlayer::COLORED_TYPE::GREY;
    bool successful = true;

    void help() const;

    string colored_token(string token) const;
    string colored_tile(Tile tile, bool colored = true) const;
    string colored_tiles(Tiles tiles) const;
    void print_state(const State& state) const;

    void options(const State& state);

public:
    void set_color_type(TerminalPlayer::COLORED_TYPE color_type);

    Action play(const State& state) override;

    void error(string error) override;

    string player_type() const override;

    std::shared_ptr<Observer> observer() override;
    void end_game(const State& state, ushort winner_position) override;
};

#endif //TERMINAL_PLAYER
