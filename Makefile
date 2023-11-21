chip8: main.o operations.o
	g++ main.o operations.o -o chip8 -lSDL2

main.o: main.cpp
	g++ -c main.cpp 

operations.o: operations.cpp
	g++ -c operations.cpp

clean:
	rm *.o 
