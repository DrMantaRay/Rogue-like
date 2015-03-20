# Makefile

CFLAGS= -std=c11

rogue: pokemon.o hero.o battle.o rogue.o maze.o mapadds.o window.o items.o list.o -lncurses
list.o: list.c rogue.h
items.o: items.c rogue.h
hero.o: hero.c rogue.h
battle.o:  battle.c rogue.h
rogue.o: rogue.c rogue.h
maze.o: maze.c rogue.h
mapadds.o: mapadds.c rogue.h
window.o: window.c rogue.h
pokemon.o: pokemon.c rogue.h

SRC=rogue.c hero.c battle.c maze.c mapadds.c window.c pokemon.c items.c list.c 

depend:
	makedepend -Y ${SRC} &> /dev/null




