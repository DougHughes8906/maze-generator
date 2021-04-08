CXX 		= g++
CXXFLAGS 	= -std=c++0x -Wall 
TEST_OBJECTS 	= renderMaze.o tests.o generateMaze.o findSolutionLength.o \
		  findStart.o 
CREATE_OBJECTS 	= renderMaze.o createMaze.o generateMaze.o \
		  findSolutionLength.o findStart.o


createMaze : $(CREATE_OBJECTS)
	$(CXX) $(CXXFLAGS) -o createMaze $(CREATE_OBJECTS)

tests : $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o tests $(TEST_OBJECTS)

renderMaze.o : renderMaze.cpp renderMaze.h maze.h
	$(CXX) $(CXXFLAGS) -c renderMaze.cpp

generateMaze.o : generateMaze.cpp maze.h generateMaze.h
	$(CXX) $(CXXFLAGS) -c generateMaze.cpp

tests.o : tests.cpp renderMaze.h maze.h generateMaze.h findSolutionLength.h
	$(CXX) $(CXXFLAGS) -c tests.cpp

createMaze.o : createMaze.cpp renderMaze.h generateMaze.h
	$(CXX) $(CXXFLAGS) -c createMaze.cpp

findSolutionLength.o : findSolutionLength.cpp maze.h findStart.h \
		       findSolutionLength.h
	$(CXX) $(CXXFLAGS) -c findSolutionLength.cpp

findStart.o : findStart.cpp maze.h findStart.h
	$(CXX) $(CXXFLAGS) -c findStart.cpp

clean :
	rm *.o tests createMaze 
