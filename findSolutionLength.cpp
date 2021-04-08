#include <queue>
#include <vector>
#include <utlity>
#include "findSolutionLength.h"
#include "findStart.h"
#include "maze.h"

// helper function to addOpenAdjacent
// pushes a location into next_locs if it is "valid"
// valid meaning it is a path area and is unexplored
void pushIfValid(std::queue<std::pair<int, int>> &next_locs,
                 const maze::bool_grid_t &maze, 
                 const maze::bool_grid_t &explored,
                 const std::pair<int, int> &location) {
  if (!maze[location.first][location.second] &&
      !explored[location.first][location.second]) {
    next_locs.push(location);
  } 
}

// helper function to findSolutionLength
// Adds any valid open, unexplored path locations adjacent
// to the given location to the next_locs queue
void addOpenAdjacent(std::queue<std::pair<int, int>> &next_locs,
                     const maze::bool_grid_t &maze, 
                     const maze::bool_grid_t &explored,
                     const std::pair<int, int> &location) {
  int left_col{ location.second - 1 };
  if (left_col >= 0) {
    std::pair<int, int> left_loc(location.first, left_col);
    pushIfValid(next_locs, maze, explored, left_loc);
  }

  int right_col{ location.second + 1 };
  if (right_col < maze[location.first].size()) {
    std::pair<int, int> right_loc(location.first, right_col);
    pushIfValid(next_locs, maze, explored, right_loc); 
  }

  int row_above{ location.first - 1 };
  if (row_above >= 0) {
    std::pair<int, int> above_loc(row_above, location.second);
    pushIfValid(next_locs, maze, explored, above_loc);
  }

  int row_below{ location.first + 1 };
  if (row_below < maze.size()) {
    std::pair<int, int> below_loc(row_below, location.second);
    pushIfValid(next_locs, maze, explored, below_loc);
  } 
}


// finds the length of the shortest solution path in the maze.
// If there is no solution, returns -1
int findSolutionLength(const maze::bool_grid_t &maze) {
  auto start_loc{ findStart(maze) };

  // perform a breadth first search
  
  // mark another grid with explored locations
  maze::bool_grid_t explored;
  for (const auto &row : maze) {
    explored.push_back(std::vector<bool>(row.size(), false));
  }

  explored[start_loc.first][start_loc.second] = true;

  std::queue<std::pair<int, int>> next_locs;

  addOpenAdjacent(next_locs, maze, explored, start_loc);

  const int first_row{ 0 };
  const int last_row{ static_cast<int>(maze.size() - 1) };
  const int first_col{ 0 };
  const int last_col{ static_cast<int>(maze[last_row].size() - 1) };

  int path_length{ 1 };

  // perform the search
  while(!next_locs.empty()) {
    auto cur_loc{ next_locs.front() };
    next_locs.pop();
    explored[cur_loc.first][cur_loc.second] = true;
    ++path_length;

    if (cur_loc.first == first_row || cur_loc.first == last_row ||
        cur_loc.second == first_col || cur_loc.second == last_col) {
      return path_length;
    } 

    addOpenAdjacent(next_locs, maze, explored, cur_loc);
  }

  return -1;
}


