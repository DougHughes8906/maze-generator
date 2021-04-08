#include <iostream>
#include <random>
#include "maze.h"
#include "renderMaze.h"
#include "generateMaze.h"
#include "findSolutionLength.h"

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

// takes a maze size and returns the corresponding side length 
// for the maze
int getSideLen(maze::MazeSize size) {
  int side_len{ 1 };
  switch(size) {
    case maze::MazeSize::small:
      side_len = maze::SMALL_SIDE_LEN;
      break;
    case maze::MazeSize::medium:
      side_len = maze::MEDIUM_SIDE_LEN;
      break;
    case maze::MazeSize::large:
      side_len = maze::LARGE_SIDE_LEN;
      break;
    default:
      break;  
  }
  return side_len;
}

// builds a pool of mazes of the given size, sorts them by 
// the length of the fastest solution to each of the mazes
// in ascending order
void buildMazePool(std::vector<maze::maze_info_t> &maze_pool, 
                   int num_mazes,
                   maze::MazeSize size,   
                   std::mt19937 &random_engine) {
  int side_len{ getSideLen(size) };


  for (int i{ 0 }; i < num_mazes; ++i) {
    maze_pool.push_back(std::make_pair(generateMaze(side_len, random_engine),
                                       0));
    maze_pool[i].second = findSolutionLength(maze_pool[i].first);
  }
}

// takes a pool of mazes and chooses/returns one maze based on the 
// desired difficulty level
// PRECONDITION: the maze_pool is sorted in ascending order by the 
// difficulty of the mazes
maze::bool_grid_t chooseMaze(const std::vector<maze::maze_info_t> &maze_pool,
                             maze::MazeDifficulty difficulty) {
  return maze_pool[0].first;
}
 
// creates a maze of the given size and difficulty
maze::bool_grid_t createMaze(maze::MazeSize size, 
                             maze::MazeDifficulty difficulty,
                             std::mt19937 &random_engine) {  
  int num_mazes{ 1000 };

  std::vector<maze::maze_info_t> maze_pool;

  buildMazePool(maze_pool, num_mazes, size, random_engine);

  return chooseMaze(maze_pool, difficulty); 
}

int main() {
  // set up random engine
  std::random_device rd;
  std::mt19937 random_engine(rd());  

  auto maze_size{ getMazeSize() };
  auto maze_difficulty{ getMazeDifficulty() };

  std::cout << "\n";
  printMaze(renderMaze(createMaze(maze_size, maze_difficulty, 
                                  random_engine)));   
  return 0;
}
