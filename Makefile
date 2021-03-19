OBJECTS = renderMaze.o tests.o

tests : $(OBJECTS)
	g++ -std=c++0x -o tests $(OBJECTS)

renderMaze.o : renderMaze.cpp renderMaze.h
	g++ -std=c++0x -c renderMaze.cpp

tests.o : tests.cpp renderMaze.h
	g++ -std=c++0x -c tests.cpp

clean :
	rm tests $(OBJECTS)
