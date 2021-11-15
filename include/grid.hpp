#pragma once
#include <vector>

enum class State{
    EMPTY,
    PRESENT,
    OCCUPIED,
    DESTROYED,
};

struct Grid{
    size_t x, y;
    std::vector<std::vector<State>> states;
    Grid(const size_t& rows = 10, const size_t& cols = 10);

    void draw();
};

