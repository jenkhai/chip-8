chip8: chip8.o
	g++ chip8.o -o chip8

chip8.o: chip8.cpp
	g++ -c chip8.cpp

clean:
	rm *.o 
