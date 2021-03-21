#include <utility>
#include <vector>
#include <cassert>
#include "maze.h"
#include "generateMaze.h"

std::pair<int, int> chooseStartLocation(int side_len) {
  return std::make_pair(0, 0);
}

void buildSolutionPath(maze::bool_grid_t &maze, 
                       std::pair<int, int> start_loc) {
  return;
} 

void buildBranches(maze::bool_grid_t &maze, std::pair<int, int> start_loc) {
  return;
}

maze::bool_grid_t generateMaze(int side_len) {
  assert(side_len > 0);

  maze::bool_grid_t new_maze( side_len, std::vector<bool>(side_len, true) );
 
  if (side_len < maze::MIN_SIDE_LEN) {
    return new_maze;
  }

  auto start_loc{ chooseStartLocation(side_len) };

  buildSolutionPath(new_maze, start_loc);

  buildBranches(new_maze, start_loc);

  return new_maze;  
}
