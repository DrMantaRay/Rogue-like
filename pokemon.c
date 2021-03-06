#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ncurses.h>
#include "rogue.h"

//gets the pokemon name, indexed by number, out of the list of all pokemon.
char *accesspokename(int i,pokemons pokelist) {
  if (i>25) {
    return NULL;
  }
  pokemon_cons current=pokelist->front;
  int j = i;
  while (j>0) {
    j -= 1;
    current=current->next;
  }
  return current->contents->name;
}

//move initializer
amove new_move(char *name, char *type, int tohit, int power) {
  amove result = (amove) malloc(sizeof(move_store));
  assert(result != NULL);
  result->name = name;
  result->type = type;
  result->tohit = tohit;
  result->power = power;
  return result;
}

void free_move(amove m) {
  free((void *) m);
}

//finds move indexed by name from moves format
amove find_move(char *movename, moves movelist) {
  move_cons current = movelist->front;
  while (current != NULL) {
    if (!strcmp(current->contents->name,movename) != 0) {
      return current->contents;
    }
    current = current->next;
  }
  fprintf(stderr,"Error: Move %s not found", movename);
  return NULL;
}

//move_cons initializer
move_cons new_move_cons(amove m, move_cons next) {
  move_cons result = (move_cons) malloc(sizeof(move_cons_store));
  assert(result != NULL);
  result->contents = m;
  result->next = next;
  return result;
}

void free_move_cons(move_cons m) {
  free((void *) m);
}

//moves initializer
moves new_moves() {
  moves result = (moves) malloc(sizeof(moves_store));
  assert(result != NULL);
  result->front = NULL;
  return result;
}

//attaches move to the front of the movelist
void add_move(amove m, moves movelist) {
  move_cons newcons = new_move_cons(m,movelist->front);
  movelist->front = newcons;
}
  
//number of moves in movelist
int num_moves(moves m) {
  int result = 0;
  move_cons current = m->front;
  while (current != NULL) {
    result++;
    current = current->next;
  }
  return result;
}

void free_moves(moves m) {
  move_cons current = m->front;
  while (current != NULL) {
    free_move(current->contents);
    free_move_cons(current);
    current = current->next;
  }
  free((void *) m);
}

//reads moves from appropriately formatted text file.
moves read_moves(FILE *fp) {
  moves result = new_moves();
  char *line=malloc(400);
  while (fgets(line,400,fp)) {
    char *name=malloc(20);
    char *type = malloc(20);
    int tohit;
    int power;
    sscanf(line,"%s %s %d %d\n",name,type,&tohit,&power);
    amove newmove = new_move(name,type, tohit, power);
    add_move(newmove,result);
  }
  return result;
}

void free_pokemon(pokemonT m) {
  free((void *) m);
}

//Pokemon functions follow same format as move functions
void add_pokemon(pokemonT m, pokemons movelist) {
  pokemon_cons newcons = new_pokemon_cons(m,movelist->front);
  movelist->front = newcons;
}

pokemon_cons new_pokemon_cons(pokemonT m, pokemon_cons next) {
  pokemon_cons result = (pokemon_cons) malloc(sizeof(pokemon_cons_store));
  assert(result != NULL);
  result->contents = m;
  result->next = next;
  return result;
}

void free_pokemon_cons(pokemon_cons m) {
  free((void *) m);
}

pokemons new_pokemons() {
  pokemons result = (pokemons) malloc(sizeof(pokemons_store));
  assert(result != NULL);
  result->front = NULL;
  return result;
}

pokemonT new_pokemonT(char *name, char form, char *type, char **movelist, int baseatk, int basedef, int basehp) {
  pokemonT result = (pokemonT) malloc(sizeof(pokemonT_store));
  assert(result != NULL);
  result->name = name;
  result->form = form;
  result->type = type;
  result->movelist = movelist;
  result->baseatk = baseatk;
  result->basedef = basedef;
  result->basehp = basehp;
  return result;
}

//This is the only wrinkle cf moves: need additional thing to read the list of moves for the pokemon; comma separated.
pokemons read_pokemons(FILE *fp) {
  char*line=malloc(600);
  pokemons result = new_pokemons();
  while (fgets(line,600,fp)) {
    char *name=malloc(20);
    char display;
    char *type=malloc(20);
    char *movelist=malloc(400);
    int baseatk;
    int basedef;
    int basehp;
    sscanf(line,"%s %c %s %s %d %d %d\n",name,&display,type,movelist,&baseatk,&basedef,&basehp);
    char** rmovelist=(char**)calloc(20,20);
    char *token=strtok(movelist,",");
    int counter =0;
    while ( token != NULL) {
      rmovelist[counter]=token;
      token=strtok(NULL,",");
      counter++;
    }
    pokemonT newpoke = new_pokemonT(name,display, type, rmovelist, baseatk, basedef, basehp);
    add_pokemon(newpoke,result);
  }
  return result;
}

pokemonT find_pokemon(char *poke, pokemons pokelist) {
  pokemon_cons current = pokelist->front;
  while (current != NULL) {
    if (!strcmp(current->contents->name,poke) != 0) {
      return current->contents;
    }
    current = current->next;
  }
  fprintf(stderr,"Error: Pokemon %s not found", poke);
  return NULL;
}

void free_pokemons(pokemons m) {
  pokemon_cons current = m->front;
  while (current != NULL) {
    free_pokemon(current->contents);
    free_pokemon_cons(current);
    current = current->next;
  }
  free((void *) m);
}

//a monster is what you actually see, an instantiation of a pokemon
monster init_monster(int level, char *poketype, pokemons pokelist) {
  monster result = (monster) malloc(sizeof(monster_store));
  assert(result != NULL);
  result->poketype=find_pokemon(poketype,pokelist);
  result->maxhp = (result->poketype->basehp)*(2 + level);
  result->curhp = result->maxhp;
  result->atk = (result->poketype->baseatk)*(2+level);
  result->def = (result->poketype->basedef)*(2+level);
  result->level = level;
  return result;
}

void freemonster(monster m) {
  free((void *) m);
}

//helper function to compute length of array of strings.
int leng(char** array) {
  int count = 0;
  while(array[count]) count++;
  return count;
}
  
