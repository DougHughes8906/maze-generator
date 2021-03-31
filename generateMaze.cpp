#include <utility>
#include <vector>
#include <cassert>
#include <random>
#include <iostream>
#include "maze.h"
#include "generateMaze.h"

// information that is used related to a location to decide if it 
// will be opened as path area
struct ChoiceInfo {
  std::pair<int, int> location;
  bool available;
  int probability;
  maze::Direction direction;
  ChoiceInfo(std::pair<int, int> loc, bool is_avail, int prob,
             maze::Direction new_direction) {
    location = loc;
    available = is_avail;
    probability = prob;
    direction = new_direction;
  }
};

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

// Returns a new location based on a current location, and the direction
// to move (the objective direction, ex. towards the top of the maze)
// NOTE: this function does not check to see if the new location is "valid",
// i.e. within the boundaries of a maze
std::pair<int, int> getAdjacent(std::pair<int, int> cur_loc, 
                                maze::Direction direction_to_move) {
  auto next_loc{ cur_loc };

  switch(direction_to_move) {
    case maze::Direction::up:
      --(next_loc.first);
      break;
    case maze::Direction::down:
      ++(next_loc.first);
      break;
    case maze::Direction::left:
      --(next_loc.second);
    case maze::Direction::right:
      ++(next_loc.second);
      break;
    default:
      break;  
  };
 
  return next_loc;
}

// determines if path can be opened in a given location. It cannot be opened
// if there will be two whitespaces next to it
// PRECONDITION: the location is a valid location in the maze
bool canOpenPath(std::pair<int, int> location, const maze::bool_grid_t &maze) {
  return true;
}

// randomly chooses a location based on the probabilities passed in with
// the ChoiceInfo structs. Only considers a choice if the location is 
// available. 
// If only one choice is available that location is returned.
// If only one of right and left is available, the remaining choice gets
//  the probability of both. 
// If no choice is available, a location of (0, 0) is returned.
std::pair<int, int> chooseNextLocation(const ChoiceInfo &forward_info,
                                       const ChoiceInfo &right_info,
                                       const ChoiceInfo &left_info,
                                       std::mt19937 &random_engine) {
  return forward_info.location;
}

bool isBorder(std::pair<int, int> location, const maze::bool_grid_t &maze) {
  return true;
} 

// find a new direction based on a current direction and where you
// are turning. Ex. currently facing the top of the maze and turning right
// - now facing the right of the maze
maze::Direction findDirection(maze::Direction cur_direction, 
                              maze::RelativeDirection where_to_move) {
  // a turn towards the top of the maze
  if ((cur_direction == maze::Direction::up && 
       where_to_move == maze::RelativeDirection::forward) ||
      (cur_direction == maze::Direction::left &&
       where_to_move == maze::RelativeDirection::right) ||
      (cur_direction == maze::Direction::right &&
       where_to_move == maze::RelativeDirection::left)) {
    return maze::Direction::up;    
  } // a turn towards the bottom of the maze
  else if ((cur_direction == maze::Direction::down &&
            where_to_move == maze::RelativeDirection::forward) ||
           (cur_direction == maze::Direction::left &&
            where_to_move == maze::RelativeDirection::left) ||
           (cur_direction == maze::Direction::right &&
            where_to_move == maze::RelativeDirection::right)) {
    return maze::Direction::down;
  } // a turn towards the right border of the maze
  else if ((cur_direction == maze::Direction::right &&
            where_to_move == maze::RelativeDirection::forward) ||
           (cur_direction == maze::Direction::up &&
            where_to_move == maze::RelativeDirection::right) ||
           (cur_direction == maze::Direction::down &&
            where_to_move == maze::RelativeDirection::left)) {
    return maze::Direction::right;
  } // a turn towards the left border of the maze
  else {
    return maze::Direction::left;
  }
}

// builds a single path from one border location to another that can be
// used to solve the maze
void buildSolutionPath(maze::bool_grid_t &maze, 
                       std::pair<int, int> start_loc,
                       std::mt19937 &random_engine) {
  const int maze_side_len{ static_cast<int>(maze.size()) };

  // open the starting location
  openLocation(start_loc, maze); 

  // find starting direction
  maze::Direction start_direction
  { 
    getStartDirection(start_loc, maze_side_len) 
  };

  // determine the next path location based on the starting 
  // direction (one space forward from starting)
  auto cur_loc{ getAdjacent(start_loc, start_direction) }; 

  // open the next location
  openLocation(cur_loc, maze);  
  auto cur_direction{ start_direction };

  // probabilities of moving in each direction
  // the probability for any given direction is that value over the
  // sum of all three values
  int forward_probability{ 6 };
  int right_probability{ 2 };
  int left_probability{ 2 };

  // flag used to determine when the solution path building is finished
  bool finished{ false };
  while (!finished) {
    auto forward_direction{ cur_direction };
    auto forward_loc{ getAdjacent(cur_loc, forward_direction) };
    auto right_direction{ findDirection(cur_direction, 
                                        maze::RelativeDirection::right) }; 
    auto right_loc{ getAdjacent(cur_loc, right_direction) };
    auto left_direction{ findDirection(cur_direction,
                                       maze::RelativeDirection::left) }; 
    auto left_loc{ getAdjacent(cur_loc, left_direction) }; 

    bool forward_avail{ canOpenPath(forward_loc, maze) };
    bool right_avail{ canOpenPath(right_loc, maze) };
    bool left_avail{ canOpenPath(left_loc, maze) }; 

    ChoiceInfo forward_info(forward_loc, forward_avail, forward_probability,
                            forward_direction);
    ChoiceInfo right_info(right_loc, right_avail, right_probability,
                          right_direction);
    ChoiceInfo left_info(left_loc, left_avail, left_probability,
                         left_direction);
    cur_loc = chooseNextLocation(forward_info, right_info, left_info,
                                 random_engine);
 
    openLocation(cur_loc, maze);
    finished = isBorder(cur_loc, maze);
  }  
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

  buildSolutionPath(new_maze, start_loc, random_engine);

  buildBranches(new_maze, start_loc);

  return new_maze;  
}
