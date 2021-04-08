#include <string>
#include <vector>
#include <iostream>
#include <random>
#include "maze.h"
#include "renderMaze.h"
#include "generateMaze.h"
#include "findSolutionLength.h"

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
  
  const maze::string_grid_t expected_grid{
    { maze::wall_chtr, maze::wall_chtr, maze::path_chtr, maze::wall_chtr },
    { maze::wall_chtr, maze::wall_chtr, maze::path_chtr, maze::wall_chtr },
    { maze::wall_chtr, maze::path_chtr, maze::path_chtr, maze::wall_chtr },
    { maze::wall_chtr, maze::path_chtr, maze::wall_chtr, maze::wall_chtr }
  };

  const std::string test_fail_message{ "**TEST FAIL** renderMaze function "
    "fails to accurately render the maze." };

  testTrue(gridsEqual(renderMaze(test_grid), expected_grid),
    test_fail_message);
}

// checks to see if a single maze is valid
bool checkValid(const maze::bool_grid_t &maze) {  
  int path_length{ findSolutionLength(maze) };
  return (path_length != -1);
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
  const std::string error_start{ "**TEST FAILED** invalid maze: \n" };

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
