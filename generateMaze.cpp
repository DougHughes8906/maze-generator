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

  std::uniform_int_distribution<int> border_distribution(maze::MIN_BORDER_INT, 
                                                         maze::MAX_BORDER_INT);

  Border chosen_border{ 
    static_cast<maze::Border>(border_distribution(random_engine))
  };
   
  const int MIN_INDEX{ 1 };
  const int MAX_INDEX{ side_len - 2 };

  std::uniform_int_distribution<int> index_distribution(MIN_INDEX, 
                                                        MAX_INDEX);

  int chosen_index{ index_distribution(random_engine) };

  switch(chosen_border) {
    case maze::Border::top:
      start_loc.second = chosen_index; 
      break; 
    case maze::Border::bottom:
      start_loc.first = side_len - 1;
      start_loc.second = chosen_index;
      break;
    case maze::Border::left:
      start_loc.first = chosen_index;
      break;
    case maze::Border::right:
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

// sets a given location in the maze to false indicating that it is
// a path area of the maze
// PRECONDITION: the given location is a valid location within the maze
void openLocation(std::pair<int, int> location, maze::bool_grid_t &maze) {
  maze[location.first][location.second] = false;
}

// builds a single path from one border location to another that can be
// used to solve the maze
void buildSolutionPath(maze::bool_grid_t &maze, 
                       std::pair<int, int> start_loc) {
  const int maze_side_len{ static_cast<int>(maze.size()) };

  // open the starting location
  openLocation(start_loc, maze); 

  // find starting direction
  maze::Direction start_direction{ getStartDirection(start_loc) };

  // determine the next path location based on the starting 
  // direction
  auto next_loc{ start_loc };

  switch(start_direction) {
    case maze::up:
      --(next_loc.first);
      break;
    case maze::down:
      ++(next_loc.first);
      break;
    case maze::left:
      --(next_loc.second);
      break;
    case maze::right:
      ++(next_loc.first);
      break;
    default:
      break;
  };

  // open the next location
  openLocation(next_loc, maze);  

  
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
