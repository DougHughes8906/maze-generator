#ifndef RENDER_MAZE_H
#define RENDER_MAZE_H

#include <vector>
#include "maze.h"

maze::string_grid_t renderMaze(const maze::bool_grid_t &maze);
void printMaze(const maze::string_grid_t &maze);

#endif
