#ifndef GENERATE_MAZE_H
#define GENERATE_MAZE_H

#include <random>
#include "maze.h"

maze::bool_grid_t generateMaze(int side_len, std::mt19937 &random_engine);

#endif
