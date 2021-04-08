#include <utility>
#include "maze.h"
#include "findStart.h"

// checks to see if there are two (and only two) path areas
// on the borders of the maze. If not, returns (-1, -1).
// Otherwise returns the (row, column) for one of those two
// spots (either could be considered the maze start)
std::pair<int, int> findStart(const maze::bool_grid_t &maze) {
  std::pair<int, int> start_loc(-1, -1);
  int num_openings{ 0 };

  // first column and last column
  for (int i{ 0 }; i < maze.size(); ++i) {
    // false value indicates path
    if (!maze[i][0]) {
      start_loc.first = i;
      start_loc.second = 0;
      ++num_openings;
    }
    int last_col{ static_cast<int>(maze[i].size() - 1) };
    if (!maze[i][last_col]) {
      start_loc.first = i;
      start_loc.second = last_col;
      ++num_openings;
    }
  }

  // first row
  for (int j{ 1 }; j < maze[0].size() - 1; ++j) {
    if (!maze[0][j]) {
      start_loc.first = 0;
      start_loc.second = j;
      ++num_openings;
    }
  } 

  // last row
  int last_row{ static_cast<int>(maze.size() - 1) };
  for (int j{ 1 }; j < maze[last_row].size() - 1; ++j) {
    if (!maze[last_row][j]) {
      start_loc.first = last_row;
      start_loc.second = j;
      ++num_openings;
    }
  }
 
  if (num_openings != 2) {
    start_loc.first = -1;
    start_loc.second = -1; 
  }

  return start_loc;
}


