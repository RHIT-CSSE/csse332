all: game game_solution

.PHONY: all clean

CFLAGS+=-ggdb
LDLIBS+=-pthread

game: game.o

game_solution: game_solution.o

clean:
	rm -rf *.o game game_solution
