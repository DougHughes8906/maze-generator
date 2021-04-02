#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <queue>
#include "maze.h"
#include "renderMaze.h"
#include "generateMaze.h"

/*
  Tests that two 2D vectors have the same number of rows, the 
  same number of columns in each row, and the same value in
  every cell.
*/
template <typename T>
bool gridsEqual(const std::vector<std::vector<T>> &grid_a, 
                const std::vector<std::vector<T>> &grid_b) {
  if (grid_a.size() != grid_b.size()) {
    return false;
  }

  int num_rows{ static_cast<int>(grid_a.size()) };
  for (int i{ 0 }; i < num_rows; ++i) {
    if (grid_a[i].size() != grid_b[i].size()) {
      return false;
    }

    int num_cols{ static_cast<int>(grid_a[i].size()) };
    for (int j{ 0 }; j < num_cols; ++j) {
      if (grid_a[i][j] != grid_b[i][j]) {
        return false;
      }
    }
  } 
  return true;
}

void testTrue(bool test_value, const std::string &error_message) {
  if (!test_value) {
    std::cout << error_message << "\n";
  }
}

// tests the renderMaze function
void testRenderMaze() {
  const maze::bool_grid_t test_grid{
    { true, true, false, true },
    { true, true, false, true },
    { true, false, false, true },
    { true, false, true, true }
  };
  
  const maze::char_grid_t expected_grid{
    { maze::wall_chtr, maze::wall_chtr, maze::path_chtr, maze::wall_chtr },
    { maze::wall_chtr, maze::wall_chtr, maze::path_chtr, maze::wall_chtr },
    { maze::wall_chtr, maze::path_chtr, maze::path_chtr, maze::wall_chtr },
    { maze::wall_chtr, maze::path_chtr, maze::wall_chtr, maze::wall_chtr }
  };

  const std::string test_fail_message{ "Test fail: renderMaze function "
    "fails to accurately render the maze." };

  testTrue(gridsEqual(renderMaze(test_grid), expected_grid),
    test_fail_message);
}

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

// helper function to findPath
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

// returns true if the end of the maze can be reached
// from the start and false otherwise.
//
// PRECONDITION: there is a single start location and
// a single end location
bool findPath(const maze::bool_grid_t &maze, 
              const std::pair<int, int> &start_loc) {
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

  // perform the search
  while(!next_locs.empty()) {
    auto cur_loc{ next_locs.front() };
    next_locs.pop();
    explored[cur_loc.first][cur_loc.second] = true;

    if (cur_loc.first == first_row || cur_loc.first == last_row ||
        cur_loc.second == first_col || cur_loc.second == last_col) {
      return true;
    } 

    addOpenAdjacent(next_locs, maze, explored, cur_loc);
  }

  return false;
}

// checks to see if a single maze is valid
bool checkValid(const maze::bool_grid_t &maze) {
  auto start_loc{ findStart(maze) };

  if (start_loc.first != -1) {
    return findPath(maze, start_loc);
  }

  return false;
}


// tests that the generateMaze function generates "valid"
// mazes. A maze is considered valid if it has a non-adjacent
// entrance and exit, and there is a path from the entrance
// to the exit.
void testValidMaze(std::mt19937 &random_engine) {
  
  auto small_maze{ generateMaze(maze::MIN_SIDE_LEN, random_engine) };
  checkValid(small_maze);

  constexpr int num_test_runs{ 100 };
  std::uniform_int_distribution<int> distribution(maze::MIN_SIDE_LEN,
                                                  maze::MAX_SIDE_LEN);
  const std::string error_start{ "Test failed, invalid maze: \n" };

  for (int i{ 0 }; i < num_test_runs; ++i) {
    int num_sides{ distribution(random_engine) }; 
    auto test_maze{ generateMaze(num_sides, random_engine) };
    bool maze_valid{ checkValid(test_maze) };
    if (!maze_valid) {
      std::cout << error_start; 
      printMaze(renderMaze(test_maze)); 
      std::cout << "\n\n";
    }
    else {
      std::cout << "Test passed, valid maze:\n";
      printMaze(renderMaze(test_maze)); 
      std::cout << "\n\n";
    } 
  }
}

int main() {
  // set up random engine
  std::random_device rd;
  std::mt19937 random_engine(rd());  
 
  testRenderMaze(); 
  testValidMaze(random_engine);

  return 0;
}
