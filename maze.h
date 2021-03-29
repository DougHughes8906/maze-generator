#ifndef MAZE_H
#define MAZE_H

namespace maze
{
  using char_grid_t = std::vector<std::vector<char>>;
  using bool_grid_t = std::vector<std::vector<bool>>; 

  constexpr char wall_chtr{ 'X' };
  constexpr char path_chtr{ ' ' };
  constexpr int MIN_SIDE_LEN{ 3 };
  constexpr int MAX_SIDE_LEN{ 80 };
  static constexpr int MIN_BORDER_INT{ 0 };
  static constexpr int MAX_BORDER_INT{ 3 };

  enum class Border
  {
    top,
    bottom,
    left,
    right
  };

  enum class Direction
  {
    up,
    down,
    left,
    right
  };
}

#endif
