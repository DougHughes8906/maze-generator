#include <utility>
#include <vector>
#include <cassert>
#include <random>
#include <iostream>
#include "maze.h"
#include "generateMaze.h"

// randomly chooses the start location for the maze along one of the borders
// of the maze. The corners of the maze are excluded as possible starting
// locations.
std::pair<int, int> chooseStartLocation(int side_len, 
                                        std::mt19937 &random_engine) {
  std::pair<int, int> start_loc(0, 0);
 
  enum class Border
  {
    top,
    bottom,
    left,
    right
  };

  static constexpr int MIN_BORDER{ 0 };
  static constexpr int MAX_BORDER{ 3 };

  std::uniform_int_distribution<int> border_distribution(MIN_BORDER, 
                                                         MAX_BORDER);

  Border chosen_border{ 
    static_cast<Border>(border_distribution(random_engine))
  };
   
  const int MIN_INDEX{ 1 };
  const int MAX_INDEX{ side_len - 2 };

  std::uniform_int_distribution<int> index_distribution(MIN_INDEX, 
                                                        MAX_INDEX);

  int chosen_index{ index_distribution(random_engine) };

  switch(chosen_border) {
    case Border::top:
      start_loc.second = chosen_index; 
      break; 
    case Border::bottom:
      start_loc.first = side_len - 1;
      start_loc.second = chosen_index;
      break;
    case Border::left:
      start_loc.first = chosen_index;
      break;
    case Border::right:
      start_loc.first = chosen_index;
      start_loc.second = side_len - 1;
      break;
    default:
      std::cout << "Error in starting location selection: "
                << "starting side index of " << chosen_index 
                << " not valid.\n";
      break;
  };  

  return start_loc;
}

void buildSolutionPath(maze::bool_grid_t &maze, 
                       std::pair<int, int> start_loc) {
  return;
} 

void buildBranches(maze::bool_grid_t &maze, std::pair<int, int> start_loc) {
  return;
}

maze::bool_grid_t generateMaze(int side_len, std::mt19937 &random_engine) {
  assert(side_len > 0);

  maze::bool_grid_t new_maze( side_len, std::vector<bool>(side_len, true) );
 
  if (side_len < maze::MIN_SIDE_LEN) {
    return new_maze;
  }

  auto start_loc{ chooseStartLocation(side_len, random_engine) };

  buildSolutionPath(new_maze, start_loc);

  buildBranches(new_maze, start_loc);

  return new_maze;  
}
