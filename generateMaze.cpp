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

  maze::Border chosen_border{ 
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

// finds the starting direciton for a maze based on the starting location
maze::Direction getStartDirection(std::pair<int, int> start_loc, int side_len) {
  if (start_loc.first == 0) {
    return maze::Direction::down;
  } else if (start_loc.first == side_len - 1) {
    return maze::Direction::up;
  } else if (start_loc.second == 0) {
    return maze::Direction::right;
  } else {
    return maze::Direction::left;
  }
}

// Returns a new location based on a current location, a current objective
// direction (ex. facing up towards the top of the board), and a desired
// relative direction to move (ex. forward according to the current direction
// NOTE: this function does not check to see if the new location is "valid",
// i.e. within the boundaries of a maze
std::pair<int, int> getAdjacent(std::pair<int, int> cur_loc, 
                                maze::Direction cur_direction,
                                maze::RelativeDirection where_to_move) {
  auto next_loc{ cur_loc };

  // a move up towards the top of the maze
  if ((cur_direction == maze::Direction::up && 
       where_to_move == maze::RelativeDirection::forward) ||
      (cur_direction == maze::Direction::left &&
       where_to_move == maze::RelativeDirection::right) ||
      (cur_direction == maze::Direction::right &&
       where_to_move == maze::RelativeDirection::left)) {
    --(next_loc.first);
  } // a move towards the bottom of the maze
  else if ((cur_direction == maze::Direction::down &&
            where_to_move == maze::RelativeDirection::forward) ||
           (cur_direction == maze::Direction::left &&
            where_to_move == maze::RelativeDirection::left) ||
           (cur_direction == maze::Direction::right &&
            where_to_move == maze::RelativeDirection::right)) {
    ++(next_loc.first);
  } // a move towards the right border of the maze
  else if ((cur_direction == maze::Direction::right &&
            where_to_move == maze::RelativeDirection::forward) ||
           (cur_direction == maze::Direction::up &&
            where_to_move == maze::RelativeDirection::right) ||
           (cur_direction == maze::Direction::down &&
            where_to_move == maze::RelativeDirection::left)) {
    ++(next_loc.second);
  } // a move towards the left border of the maze
  else {
    --(next_loc.second);
  }

  return next_loc;
}


// builds a single path from one border location to another that can be
// used to solve the maze
void buildSolutionPath(maze::bool_grid_t &maze, 
                       std::pair<int, int> start_loc) {
  const int maze_side_len{ static_cast<int>(maze.size()) };

  // open the starting location
  openLocation(start_loc, maze); 

  // find starting direction
  maze::Direction start_direction
  { 
    getStartDirection(start_loc, maze_side_len) 
  };

  // determine the next path location based on the starting 
  // direction (one space forward from starting
  auto next_loc{ getAdjacent(start_loc, start_direction, 
                             maze::RelativeDirection::forward) }; 

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
