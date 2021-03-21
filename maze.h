#ifndef MAZE_H
#define MAZE_H

namespace maze
{
  using char_grid_t = std::vector<std::vector<char>>;
  using bool_grid_t = std::vector<std::vector<bool>>; 

  constexpr char wall_chtr{ 'X' };
  constexpr char path_chtr{ ' ' };
  constexpr int MIN_SIDE_LEN{ 3 };
}

#endif
