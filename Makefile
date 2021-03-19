CXX 		= g++
CXXFLAGS 	= -std=c++0x -Wall 
OBJECTS 	= renderMaze.o tests.o

tests : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o tests $(OBJECTS)

renderMaze.o : renderMaze.cpp renderMaze.h
	$(CXX) $(CXXFLAGS) -c renderMaze.cpp

tests.o : tests.cpp renderMaze.h
	$(CXX) $(CXXFLAGS) -c tests.cpp

clean :
	rm tests $(OBJECTS)
