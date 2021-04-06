CXX 		= g++
CXXFLAGS 	= -std=c++0x -Wall 
TEST_OBJECTS 	= renderMaze.o tests.o generateMaze.o 
CREATE_OBJECTS 	= renderMaze.o createMaze.o generateMaze.o 


createMaze : $(CREATE_OBJECTS)
	$(CXX) $(CXXFLAGS) -o createMaze $(CREATE_OBJECTS)

tests : $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o tests $(TEST_OBJECTS)

renderMaze.o : renderMaze.cpp renderMaze.h maze.h
	$(CXX) $(CXXFLAGS) -c renderMaze.cpp

tests.o : tests.cpp renderMaze.h maze.h generateMaze.h
	$(CXX) $(CXXFLAGS) -c tests.cpp

clean :
	rm *.o tests createMaze 
