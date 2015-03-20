#include <stdio.h>
#include <ncurses.h>
#include "rogue.h"
#include <stdlib.h>
#include <time.h>

item init_item(int flr) {
    item result= malloc(sizeof(item_store));
    result->name=malloc(30*sizeof(char));
    result->name="weapon";
    result->form='$';
    result->atk=rand()%flr;
    result->def=rand()%flr;
    result->maxhp=rand()%flr;
    result->curhp=rand()%(10*flr);
    result->luck=rand()%flr;
    result->exp=rand()%(5*flr);
    return result;
}

