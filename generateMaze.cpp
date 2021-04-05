#include <utility>
#include <vector>
#include <cassert>
#include <random>
#include <iostream>
#include <stack>
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

// used to indicate that no choice could be made
ChoiceInfo dummyChoice() {
  return ChoiceInfo(std::make_pair(-1, -1), false, 0, 
                    maze::Direction::up);
}

// returns true if a ChoiceInfo object is considered a dummy choice
// The distinguishing factor is that a choice has a location of (-1, -1).
// This is sufficient because it would not be a valid location in a maze
bool isDummy(const ChoiceInfo &choice) {
  return (choice.location.first == -1 && choice.location.second == -1);
}

// returns true if a given location in the maze is on a border
bool isBorder(std::pair<int, int> location, const maze::bool_grid_t &maze) { 
  int last_row_ind{ static_cast<int>(maze.size() - 1) };
  int last_col_ind{ static_cast<int>(maze[location.first].size() - 1) };
  return (location.first == 0 || location.first == last_row_ind ||
          location.second == 0 || location.second == last_col_ind); 
}

// randomly chooses a non-corner, random border location
std::pair<int, int> chooseRandomBorder(int side_len, 
                                       std::mt19937 &random_engine) {
  std::pair<int, int> random_loc(0, 0); 

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
      random_loc.second = chosen_index; 
      break; 
    case maze::Border::bottom:
      random_loc.first = side_len - 1;
      random_loc.second = chosen_index;
      break;
    case maze::Border::left:
      random_loc.first = chosen_index;
      break;
    case maze::Border::right:
      random_loc.first = chosen_index;
      random_loc.second = side_len - 1;
      break;
    default:
      std::cout << "Error in starting location selection: "
                << "starting side index of " << chosen_index 
                << " not valid.\n";
      break;
  };  

  return random_loc;
}

// returns true if two given locations are adjacent
// considered adjacent if they are the same location or they
// are 4-directionally adjacent
bool isAdjacent(std::pair<int, int> loc_one, std::pair<int, int> loc_two) { 
  if (loc_one.first == loc_two.first) {
    // same spot, or right, or left
    if (loc_one.second == loc_two.second || 
        loc_one.second == loc_two.second - 1 ||
        loc_one.second == loc_two.second + 1) {
      return true;
    } else {
      return false;
    }
  } else if (loc_one.second == loc_two.second) {
    // above or below
    return (loc_one.first == loc_two.first + 1 ||
            loc_one.first == loc_two.first - 1);
  }
  return false;
}

// randomly chooses the end location for the maze along one of the borders
// of the maze. It excludes any border locations adjacent to the start of
// the maze and the corners of the maze
std::pair<int, int> chooseEndLocation(int side_len,
                                      std::pair<int, int> start_loc, 
                                      std::mt19937 &random_engine) {
  auto end_loc{ chooseRandomBorder(side_len, random_engine) };
  while (isAdjacent(end_loc, start_loc)) {
    end_loc = chooseRandomBorder(side_len, random_engine);
  }

  return end_loc; 
}

// randomly chooses the start location for the maze along one of the borders
// of the maze. The corners of the maze are excluded as possible starting
// locations.
std::pair<int, int> chooseStartLocation(int side_len, 
                                        std::mt19937 &random_engine) {
  return chooseRandomBorder(side_len, random_engine);  
}

// sets a given location in the maze to false indicating that it is
// a path area of the maze
// PRECONDITION: the given location is a valid location within the maze
void openLocation(std::pair<int, int> location, maze::bool_grid_t &maze) {
  maze[location.first][location.second] = maze::path_val;
}

// finds a direction based on a border location. It is the 
// direction implied by imagining someone was entering the maze
// at that location
// PRECONDITION: the border_loc is actually on a border
maze::Direction getBorderDirection(std::pair<int, int> border_loc, 
                                  int side_len) {
  if (border_loc.first == 0) {
    return maze::Direction::down;
  } else if (border_loc.first == side_len - 1) {
    return maze::Direction::up;
  } else if (border_loc.second == 0) {
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
      break;
    case maze::Direction::right:
      ++(next_loc.second);
      break;
    default:
      break;  
  };
 
  return next_loc;
}

// returns true if a given location in the given maze is a path location
// PRECONDITION: the location is a valid location in the maze
bool isPath(std::pair<int, int> location, const maze::bool_grid_t &maze) {
  return (maze[location.first][location.second] == maze::path_val); 
}

// determines if path can be opened in a given location. It cannot be opened
// if there will be two path spaces next to it
// It also cannot be opened if the space is already opened
// PRECONDITION: the location is a valid location in the maze
bool canOpenPath(std::pair<int, int> location, const maze::bool_grid_t &maze) {
  // ensure the space itself is not already opened and is not on a border
  if (isPath(location, maze) || isBorder(location, maze)) {
    return false;
  } 

  int num_path_adjacent{ 0 };

  // check up
  if (location.first > 0) {
    auto up_loc{ getAdjacent(location, maze::Direction::up) };
    if (isPath(up_loc, maze)) {
      ++num_path_adjacent;
    }
  } 
  // check down
  int last_row_ind{ static_cast<int>(maze.size() - 1) };
  if (location.first < last_row_ind) {
    auto down_loc{ getAdjacent(location, maze::Direction::down) };
    if (isPath(down_loc, maze)) {
      ++num_path_adjacent;
    }
  }
  // check right
  int last_col_ind{ static_cast<int>(maze[location.first].size() - 1) };
  if (location.second < last_col_ind) {
    auto right_loc{ getAdjacent(location, maze::Direction::right) };
    if (isPath(right_loc, maze)) {
      ++num_path_adjacent;
    }
  }
  // check left
  if (location.second > 0) {
    auto left_loc{ getAdjacent(location, maze::Direction::left) };
    if (isPath(left_loc, maze)) {
      ++num_path_adjacent;
    }
  }
  
  return (num_path_adjacent < 2); 
}

// returns true if one and only one of the choices are available
bool onlyOneAvailable(const ChoiceInfo &choice_1, 
                      const ChoiceInfo &choice_2,
                      const ChoiceInfo &choice_3) {
  int num_available{ 0 };
  if (choice_1.available) {
    ++num_available;
  }
  if (choice_2.available) {
    ++num_available;
  }
  if (choice_3.available) {
    ++num_available;
  }

  return (num_available == 1);
}

// returns the first available choice (in the order of the parameters)
// if no choices are available, returns the last choice (choice_3)
ChoiceInfo availableChoice(const ChoiceInfo &choice_1, 
                           const ChoiceInfo &choice_2,
                           const ChoiceInfo &choice_3) {
  if (choice_1.available) {
    return choice_1;
  } else if (choice_2.available) {
    return choice_2;
  } else {
    return choice_3;
  } 
}

// randomly chooses a location based on the probabilities passed in with
// the ChoiceInfo structs. Only considers a choice if the location is 
// available. 
// If no choices are available, a dummy choiceInfo is returned.
// If only one choice is available that location is returned.
// If only one of right and left is available, the remaining choice gets
//  the probability of both. 
ChoiceInfo chooseNextLocation(const ChoiceInfo &forward_info,
                              const ChoiceInfo &right_info,
                              const ChoiceInfo &left_info,
                              std::mt19937 &random_engine) {
  // check to see if no choices are available
  if (!(forward_info.available || right_info.available || 
        left_info.available)) { 
    return dummyChoice();
  }
  // check to see if only one choice is available 
  else if (onlyOneAvailable(forward_info, right_info, left_info)) {
    return availableChoice(forward_info, right_info, left_info);
  } 
  // check if all available
  else if (forward_info.available && right_info.available && 
           left_info.available) {
    int probability_total{ forward_info.probability + right_info.probability +
                           left_info.probability };

    int forward_barrier{ forward_info.probability };
    int right_barrier{ forward_barrier + right_info.probability };

    std::uniform_int_distribution<int> distribution(1, probability_total);
    int choice_val{ distribution(random_engine) };

    if (choice_val <= forward_barrier) {
      return forward_info;
    } else if (choice_val <= right_barrier) {
      return right_info;
    } else {
      return left_info;
    } 
  }
  // check if forward is not available
  else if (!(forward_info.available)) {
    int probability_total{ right_info.probability + left_info.probability };
    int right_barrier{ right_info.probability }; 

    std::uniform_int_distribution<int> distribution(1, probability_total);
    int choice_val{ distribution(random_engine) };

    if (choice_val <= right_barrier) {
      return right_info;
    } else {
      return left_info;
    }
  }
  // one of right or left is not available
  else {
    auto other_probability{ right_info.probability + left_info.probability };
    auto other_choice = (right_info.available) ? right_info : left_info;
    other_choice.probability = other_probability;

    int probability_total{ forward_info.probability + 
                           other_choice.probability };
    int forward_barrier{ forward_info.probability }; 

    std::uniform_int_distribution<int> distribution(1, probability_total);
    int choice_val{ distribution(random_engine) };

    if (choice_val <= forward_barrier) {
      return forward_info;
    } else {
      return other_choice;
    } 
  }
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

// ensures a given location is within the boundaries of the given
// maze
bool inMaze(std::pair<int, int> location, const maze::bool_grid_t &maze) {
  int last_row{ static_cast<int>(maze.size() - 1) };
  int last_col{ static_cast<int>(maze[location.first].size() - 1) };

  return (location.first >= 0 && location.first <= last_row &&
          location.second >= 0 && location.second <= last_col);
}

// builds the ending for the maze
void buildEnding(maze::bool_grid_t &maze,
                 std::pair<int, int> start_loc,
                 std::mt19937 &random_engine) {
  int side_len{ static_cast<int>(maze.size()) };
  auto end_location{ chooseEndLocation(side_len, start_loc, random_engine) };
  auto end_direction{ getBorderDirection(end_location, side_len) };

  openLocation(end_location, maze);
  // if there is wall in front of the end location, clear it out
  auto next_loc{ getAdjacent(end_location, end_direction) };
  while (inMaze(next_loc, maze) && !isPath(next_loc, maze)) {
    openLocation(next_loc, maze);
    next_loc = getAdjacent(next_loc, end_direction); 
  }
}
// builds all path area for the maze, effectively creating the 
// maze from a start of a grid of wall (the path is carved out
// of the starting all-wall grid)
void buildAllPath(maze::bool_grid_t &maze, 
                  std::pair<int, int> start_loc,
                  std::mt19937 &random_engine) {
  const int maze_side_len{ static_cast<int>(maze.size()) };  

  // open the starting location
  openLocation(start_loc, maze); 

  // find starting direction
  maze::Direction start_direction
  { 
    getBorderDirection(start_loc, maze_side_len) 
  };

  // determine the next path location based on the starting 
  // direction (one space forward from starting)
  auto cur_loc{ getAdjacent(start_loc, start_direction) };  

  // open the next location
  openLocation(cur_loc, maze);  
  auto cur_direction{ start_direction };

  // all of the info on this position
  ChoiceInfo cur_info(cur_loc, true, 1, cur_direction);

  // probabilities of moving in each direction
  // the probability for any given direction is that value over the
  // sum of all three values
  int forward_probability{ 6 };
  int right_probability{ 2 };
  int left_probability{ 2 };

  // stack used to keep track of the path taken. Used during path
  // creation if a dead end is hit
  std::stack<ChoiceInfo> path_stack;
  path_stack.push(cur_info); 

  // flag used to determine when the solution path building is finished
  bool finished{ false };
  while (!finished && !path_stack.empty()) {
    auto last_choice{ path_stack.top() };

    auto forward_direction{ last_choice.direction };
    auto forward_loc{ getAdjacent(last_choice.location, forward_direction) };
    auto right_direction{ findDirection(last_choice.direction, 
                                        maze::RelativeDirection::right) }; 
    auto right_loc{ getAdjacent(last_choice.location, right_direction) };
    auto left_direction{ findDirection(last_choice.direction,
                                       maze::RelativeDirection::left) }; 
    auto left_loc{ getAdjacent(last_choice.location, left_direction) }; 

    int num_avail{ 0 };
    bool forward_avail{ canOpenPath(forward_loc, maze) };
    if (forward_avail) {
      ++num_avail;
    }
    bool right_avail{ canOpenPath(right_loc, maze) };
    if (right_avail) {
      ++num_avail;
    }
    bool left_avail{ canOpenPath(left_loc, maze) }; 
    if (left_avail) {
      ++num_avail;
    } 

    ChoiceInfo forward_info(forward_loc, forward_avail, forward_probability,
                            forward_direction);
    ChoiceInfo right_info(right_loc, right_avail, right_probability,
                          right_direction);
    ChoiceInfo left_info(left_loc, left_avail, left_probability,
                         left_direction);
    auto next_choice = chooseNextLocation(forward_info, right_info, left_info,
                                 random_engine);

    // no need to consider this choice again if there were no options
    // or only one option (in which case that one option is next_choice)
    if (num_avail < 2) {
      path_stack.pop();
    }
 
    if (!isDummy(next_choice)) {   
      openLocation(next_choice.location, maze);
      path_stack.push(next_choice); 
    } 
  }

  buildEnding(maze, start_loc, random_engine);  
} 

maze::bool_grid_t generateMaze(int side_len, std::mt19937 &random_engine) {
  assert(side_len > 0);

  maze::bool_grid_t new_maze( side_len, std::vector<bool>(side_len, 
                                                          maze::wall_val) );
 
  if (side_len < maze::MIN_SIDE_LEN) {
    return new_maze;
  }

  auto start_loc{ chooseStartLocation(side_len, random_engine) };

  buildAllPath(new_maze, start_loc, random_engine); 

  return new_maze;  
}
