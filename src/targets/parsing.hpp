#include <cstdarg>
#include <functional>
#include <map>
#include <stdexcept>
#include <string>

#include "game/player.hpp"
#include "players/first_legal_player.hpp"
#include "players/monte_carlo_player.hpp"
#include "players/random_player.hpp"

struct PlayerParameters {
    std::string key{};
    std::map<std::string, std::string> parameters{};

private:
    bool try_get(const std::string& key, std::string& value) const {
        auto it = parameters.find(key);
        if (it == parameters.end()) {
            return false;
        }
        value = (*it).second;
        return true;
    }

    template<class T>
    void set(T& value, std::function<T(std::string)> cast, std::string key_0, std::string key_1 = "", std::string key_2 = "") const {
        std::string result;
        if (try_get(key_0, result)) {
            value = cast(result);
            return;
        }
        if (key_1 == "") {
            return;
        }
        if (try_get(key_1, result)) {
            value = cast(result);
            return;
        }
        if (key_2 == "") {
            return;
        }
        if (try_get(key_2, result)) {
            value = cast(result);
            return;
        }
    }

    static std::string _str(std::string s) {
        return s;
    }

    static bool _bool(std::string s) {
        return s != "" && s != "false" && s != "0" && s != "f";
    }

    static int _int(std::string s) {
        std::string::size_type size;
        int value = std::stoi(s, &size);
        if (size != s.size()) {
            throw std::invalid_argument("'" + s + "' is not an int");
        }
        return value;
    }

    static float _float(std::string s) {
        std::string::size_type size;
        float value = std::stof(s, &size);
        if (size != s.size()) {
            throw std::invalid_argument("'" + s + "' is not an float");
        }
        return value;
    }

    void set(std::string& value, std::string key_0, std::string key_1 = "", std::string key_2 = "") const {
        set<std::string>(value, _str, key_0, key_1, key_2);
    }

    void set(bool& value, std::string key_0, std::string key_1 = "", std::string key_2 = "") const {
        set<bool>(value, _bool, key_0, key_1, key_2);
    }

    void set(float& value, std::string key_0, std::string key_1 = "", std::string key_2 = "") const {
        set<float>(value, _float, key_0, key_1, key_2);
    }

    void set(int& value, std::string key_0, std::string key_1 = "", std::string key_2 = "") const {
        set<int>(value, _int, key_0, key_1, key_2);
    }

    template<class T>
    T get(T default_value, std::string key_0, std::string key_1 = "", std::string key_2 = "") const {
        set(default_value, key_0, key_1, key_2);
        return default_value;
    }

public:
    static PlayerParameters
    parse(std::string raw_params) {
        int i = raw_params.find('{');
        int j = raw_params.find('}');
        if (i == std::string::npos && j == std::string::npos) {
            return PlayerParameters{ .key = raw_params };
        }
        if (i == std::string::npos || j == std::string::npos) {
            throw std::invalid_argument("Option format is 'key' or 'key[options]'");
        }
        PlayerParameters result{ .key = raw_params.substr(0UL, i) };
        int last = i + 1;
        while (true) {
            int next = raw_params.find(',', last);
            std::string argument = raw_params.substr(last, (next == std::string::npos ? raw_params.length() - 1 : next) - last);
            std::string key = "";
            int split = argument.find(':');
            if (split != std::string::npos) {
                key = argument.substr(0, split);
                argument = argument.substr(split + 1);
            }
            result.parameters[key] = argument;
            if (next == std::string::npos) {
                break;
            }
            last = next + 1;
        }
        return result;
    }

    std::shared_ptr<Player>
    build() const {
        if (key == "fl" || key == "first-legal") {
            return std::make_shared<FirstLegalPlayer>();
        } else if (key == "rn" || key == "rand-naive" || key == "random-naive") {
            return std::make_shared<RandomPlayer>(false);
        } else if (key == "r" || key == "rand" || key == "random") {
            return std::make_shared<RandomPlayer>(get(true, "", "s", "smart"));
        } else if (key == "mc" || key == "monte-carlo") {
            std::shared_ptr<MonteCarloPlayer> player = std::make_shared<MonteCarloPlayer>(std::make_unique<RandomPlayer>(get(true, "s", "smart")));
            set(player->rollouts, "", "r", "rollouts");
            set(player->smart, "s", "smart");
            set(player->c, "c", "C");
            set(player->until_round, "u", "round", "until_round");
            set(player->heuristic.bonus_factor, "hb", "h-bonus");
            set(player->heuristic.leading_factor, "hl", "h-leading");
            set(player->heuristic.penalty_factor, "hp", "h-penalty");
            return player;
        } else {
            throw std::invalid_argument("Unkown player type " + key);
        }
    }

    static void
    help() {
        std::string padding = "    ";
        std::cout << padding << "<players> : list of 'key' or 'key{options}' for each player, default is 'fl rn r'\n"
                  << padding << "    fl : First Legal Player\n"
                  << padding << "    r : Random Player\n"
                  << padding << "      {options} (default is r = 'r{true}')\n"
                  << padding << "        <s>, s:<s>, smart:<s> : should the random ai used for generating rollouts be smart\n"
                  << padding << "    rn : Naive Random Player (equivalent to 'r{false}')\n"
                  << padding << "    mc : Monte-Carlo Player\n"
                  << padding << "      {options} (default is mc = 'mc{1000,s:true,c:1.41,u:true,hb:1,hl:0.6,hp:0}')\n"
                  << padding << "        <r>, r:<r>, rollouts:<r> : number of rollouts\n"
                  << padding << "        s:<s>, smart:<s> : should the random ai used for generating rollouts be smart\n"
                  << padding << "        c:<c>, C:<c> : constant 'c' in UBT formula\n"
                  << padding << "        u:<u>, round:<u>, until_round:<u> : should rollouts be stopped at the end of rounds\n"
                  << padding << "        hb:<hb>, h-bonus:<hb> : if <u>, 0 <= <hb> <= 1 is bonus factor of round heuristic\n"
                  << padding << "        hl:<hl>, h-leading:<hl> : if <u>, 0 <= <hl> <= 1 is leading factor of round heuristic\n"
                  << padding << "        hp:<hp>, h-penalty:<hp> : if <u>, 0 <= <hp> <= 1 is penalty factor of round heuristic\n";
        std::cout << std::flush;
    }
};