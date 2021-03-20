#include <string>
#include <vector>
#include <iostream>
#include "maze.h"
#include "renderMaze.h"

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

int main() {		
	testRenderMaze();	

	return 0;
}
