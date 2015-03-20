#include <stdio.h>
#include <ncurses.h>
#include "rogue.h"
#include <stdlib.h>
#include <time.h>

//item initialization, with randomization of item attributes. Name randomization comes later.
item init_item(int flr) {
    item result= malloc(sizeof(item_store));
    result->name=malloc(30*sizeof(char));
    result->name="weapon";
    result->form='$';
    result->atk=rand()%(2*(flr+2));
    result->def=rand()%(2*(flr+2));
    result->maxhp=rand()%(3*(flr+2));
    result->curhp=rand()%(10*(flr+2));
    result->luck=rand()%(flr+2);
    result->exp=rand()%(5*(flr+1));
    return result;
}

//macro for displaying item pickup. accessweapname handles the choosing of a random name.
void loot(WINDOW *topwin, item it, list weplist) {
    wclear(topwin);
    mvwprintw(topwin,1,1,"You picked up %s with +%d atk + %d def +%d maxhp.",accessweapname(rand()%15,weplist),it->atk,it->def,it->maxhp);
    wrefresh(topwin);
    wgetch(topwin);
}

//accesses weapons data, in list form.
char *accessweapname(int i,list pokelist) {
  if (i>15) {
    return NULL;
  }
  listcons current=pokelist->front;
  int j = i;
  while (j>0) {
    j -= 1;
    current=current->next;
  }
  return current->contents;
}

//reads weapon names from text file into list form.
list read_names(FILE *fp) {
  list result = new_list();
  char *line=malloc(400);
  while (fgets(line,400,fp)) {
    char *name=malloc(20);
    sscanf(line,"%s\n",name);
    addtolist(name,result);
  }
  return result;
}
