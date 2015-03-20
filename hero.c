#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ncurses.h>
#include "rogue.h"

//Need to make list type.

hero new_hero(int level, int maxhp, int curhp, int atk, int def) {
  hero result = (hero) malloc(sizeof(hero_store));
  assert(result != NULL);
  result->level=level;
  result->maxhp=maxhp;
  result->curhp=curhp;
  result->atk=atk;
  result->def=def;
  return result;
}

void *free_hero(hero h) {
  free((void*) h);
}
