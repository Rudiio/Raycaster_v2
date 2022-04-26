CC =gcc
CFLAGS = -std=c11 -Wall -ggdb3
LIBFLAGS = -lSDL2 -lSDL2main -lm
EXEC = raycaster
OBJ = Game.o Graphic.o main.o raycasting.o textures.o

.Phony : $(OBJ) $(EXEC) mvo clean 

all : $(EXEC)

Game.o : src/Game.c src/Game.h
	$(CC) $(CFLAGS)  -c $< $(LIBFLAGS)

Graphic.o : src/Graphic.c src/Graphic.h
	$(CC) $(CFLAGS)  -c $< $(LIBFLAGS)

raycasting.o : src/raycasting.c src/raycasting.h
	$(CC) $(CFLAGS)  -c $< $(LIBFLAGS)

textures.o : src/textures.c src/textures.h
	$(CC) $(CFLAGS)  -c $< $(LIBFLAGS)

main.o : src/main.c
	$(CC) $(CFLAGS)  -c $< $(LIBFLAGS)

$(EXEC): $(OBJ) mvo
	$(CC) $(CFLAGS) -o $@ OBJ/*.o $(LIBFLAGS)

mvo :
	mv *.o OBJ

clean :
	rm -f $(EXEC) && rm -f OBJ/*.o 