#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>

namespace maze
{
  using char_grid_t = std::vector<std::vector<char>>;
  using bool_grid_t = std::vector<std::vector<bool>>;
  using string_grid_t = std::vector<std::vector<std::string>>; 

  const std::string wall_chtr{ "XXX" };
  const std::string path_chtr{ "   " };
  constexpr bool wall_val{ true };
  constexpr bool path_val{ false };
  constexpr int MIN_SIDE_LEN{ 3 };
  constexpr int MAX_SIDE_LEN{ 26 };
  static constexpr int MIN_BORDER_INT{ 0 };
  static constexpr int MAX_BORDER_INT{ 3 };

  enum class Border
  {
    top,
    bottom,
    left,
    right
  };

  // The objective directions on the board. Ex. "up" is toward
  // the top row of the maze (i.e. index 0 row)
  enum class Direction
  {
    up,
    down,
    left,
    right
  };

  // direction from first person perspective as if they were going
  // through the maze
  enum class RelativeDirection
  {
    forward,
    right,
    left
  };

  enum class MazeSize
  {
    small,
    medium,
    large
  };

  enum class MazeDifficulty
  {
    easy,
    medium,
    hard
  };
}

#endif
