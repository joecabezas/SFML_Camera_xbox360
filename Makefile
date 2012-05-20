all: program

program: main.cpp
	g++ -Wall main.cpp -o program -lsfml-window -lsfml-system -lGLU

test: test.cpp
	g++ -Wall test.cpp -o test -lGLU -lGL -lglut

clean:
	rm program *.o
