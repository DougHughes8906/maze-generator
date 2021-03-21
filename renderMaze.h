#ifndef RENDER_MAZE_H
#define RENDER_MAZE_H

#include <vector>
#include "maze.h"

maze::char_grid_t renderMaze(const maze::bool_grid_t &maze);
void printMaze(const maze::char_grid_t &maze);

#endif
