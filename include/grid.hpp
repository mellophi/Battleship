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
    Grid(const size_t& rows = 5, const size_t& cols = 5);

    void draw();

    //-------------------Dhairya Code----------------
    void set_grid_random();
    bool set_grid_manual(int,int);
};

