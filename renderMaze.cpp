#include <vector>
#include <iostream>
#include <string>
#include "renderMaze.h"
#include "maze.h"

bool isWall(bool value) {
  return (value == maze::wall_val);
}

maze::string_grid_t renderMaze(const maze::bool_grid_t &maze) {
  maze::string_grid_t char_maze;

  for (auto const &row : maze) {
    std::vector<std::string> char_row;
    for (auto bool_val : row) {
      char_row.push_back((isWall(bool_val) ? maze::wall_chtr : 
                                             maze::path_chtr));  
    }
    char_maze.push_back(char_row);
  }

  return char_maze;
}

void printMaze(const maze::string_grid_t &maze) {
  for (const auto &row : maze) {
    for (auto val : row) {
      std::cout << val;
    }
    std::cout << "\n";
  }
}
