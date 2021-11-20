#pragma once
#include <grid.hpp>

namespace game{
  extern void init_grids(Grid &top, Grid &bottom, Grid &pc);
  extern void gameplay(Grid &top, Grid &bottom, Grid &pc);
}