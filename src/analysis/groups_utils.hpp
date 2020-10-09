#include <queue>
#include <vector>

class GroupUtils {
public:
    static void
    subsets(std::queue<std::vector<int>>& groups, int available_players, int game_players) {
        std::vector<int> ids;
        ids.push_back(0);
        while (true) {
            if (ids.back() >= available_players) {
                ids.pop_back();
                if (ids.empty()) {
                    return;
                }
                ids.back()++;
                continue;
            }
            if (ids.size() < (size_t)game_players) {
                ids.push_back(ids.back() + 1);
                continue;
            }
            groups.push(ids);
            ids.back()++;
        }
    }

    static void
    all(std::queue<std::vector<int>>& groups, int available_players, int game_players, bool remove_all_same) {
        std::vector<int> ids;
        ids.push_back(0);
        while (true) {
            if (ids.back() >= available_players) {
                ids.pop_back();
                if (ids.empty()) {
                    return;
                }
                ids.back()++;
                continue;
            }
            if (ids.size() < (size_t)game_players) {
                ids.push_back(ids.back());
                continue;
            }
            if (remove_all_same && ids[0] != ids[game_players - 1]) {
                groups.push(ids);
            }
            ids.back()++;
        }
    }
};