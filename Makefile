all:
	g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -g
	g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -g


	g++ -c -Isrc/include -o test.o test.cpp -g
	g++ -c -Isrc/include -o window.o window.cpp -g
	g++ -o test test.o window.o -Lsrc/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -g

