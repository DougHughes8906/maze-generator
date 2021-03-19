#ifndef RENDER_MAZE_H
#define RENDER_MAZE_H

#include <vector>

using char_grid_t = std::vector<std::vector<char>>;
using bool_grid_t = std::vector<std::vector<bool>>; 

char_grid_t renderMaze(const bool_grid_t &maze);

#endif
