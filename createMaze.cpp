#include <iostream>
#include <random>
#include "maze.h"
#include "renderMaze.h"
#include "generateMaze.h"

// prompt the console user for the desired size for the maze
maze::MazeSize getMazeSize() {
  int response{ -1 };
  std::cout << "Please enter the number next to the desired size for "
            << "the maze:\n"
            << "1) small\n"
            << "2) medium\n"
            << "3) large\n";

  std::cin >> response;

  while (response < 1 || response > 3) {
    std::cout << "Please enter 1, 2, or 3.\n";
    std::cin >> response;
  }

  // MazeSize goes from 0 to 2
  --response;
  maze::MazeSize requested_size{ static_cast<maze::MazeSize>(response) };

  return requested_size;
}

// prompt the console user for the desired difficulty of the maze
maze::MazeDifficulty getMazeDifficulty() {
  int response{ -1 };
  std::cout << "Please enter the number next to the desired difficulty for "
            << "the maze:\n"
            << "1) easy\n"
            << "2) medium\n"
            << "3) hard\n";

  std::cin >> response;

  while (response < 1 || response > 3) {
    std::cout << "Please enter 1, 2, or 3.\n";
    std::cin >> response;
  }

  // MazeDifficulty goes from 0 to 2
  --response;
  maze::MazeDifficulty requested_difficulty
  { 
    static_cast<maze::MazeDifficulty>(response) 
  };

  return requested_difficulty;
}

int main() {
  // set up random engine
  std::random_device rd;
  std::mt19937 random_engine(rd());  

  auto maze_size{ getMazeSize() };
  auto maze_difficulty{ getMazeDifficulty() };  
 
  return 0;
}
