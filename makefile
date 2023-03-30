all:
	gcc src/*.c -Wall -I include/ -L lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o sdgl.exe
	./sdgl.exe
	
