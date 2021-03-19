tests : renderMaze.o tests.o
	g++ -std=c++0x -o tests renderMaze.o tests.o

renderMaze.o : renderMaze.cpp renderMaze.h
	g++ -std=c++0x -c renderMaze.cpp

tests.o : tests.cpp renderMaze.h
	g++ -std=c++0x -c tests.cpp

clean :
	rm tests renderMaze.o tests.o
