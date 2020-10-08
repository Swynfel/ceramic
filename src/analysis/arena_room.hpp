#ifndef ARENA_ROOM_HPP
#define ARENA_ROOM_HPP

#include "arena.hpp"
#include "global.hpp"

#include <memory>
#include <thread>

class ArenaRoom {
private:
    Arena* arena;
    std::vector<std::shared_ptr<AnalysisPlayer>> players;
    std::vector<std::vector<int>> results;

    std::thread thread;

    long long execution_time;

    ArenaRoom(Arena* arena);

    void run_single(std::vector<int> ids);

    void run_sequential();
    void run_async();

public:
    ArenaRoom(const ArenaRoom& arena);

    void run();

    friend void Arena::run();
};

#endif //ARENA_ROOM_HPP
