#include <string>
#include <iostream>
#include "renderMaze.h"

bool gridsEqual(const char_grid_t &grid_a, const char_grid_t &grid_b) {
	return false;
}

void testTrue(bool test_value, const std::string &error_message) {
	if (!test_value) {
		std::cout << error_message << "\n";
	}
}

void testRenderMaze() {
	const bool_grid_t test_grid{
		{ true, true, false, true },
		{ true, true, false, true },
		{ true, false, false, true },
		{ true, false, true, true }
	};

	constexpr char wall_chtr{ 'X' };
	constexpr char path_chtr{ ' ' };

	const char_grid_t expected_grid{
		{ wall_chtr, wall_chtr, path_chtr, wall_chtr },
		{ wall_chtr, wall_chtr, path_chtr, wall_chtr },
		{ wall_chtr, path_chtr, path_chtr, wall_chtr },
		{ wall_chtr, path_chtr, wall_chtr, wall_chtr }
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
