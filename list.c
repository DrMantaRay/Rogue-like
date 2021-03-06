#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ncurses.h>
#include "rogue.h"


//checks whether a string is in the list data-type by cycling through
int is_move(char *move, list movelist) {
	listcons current = movelist->front;
	while (current != NULL) {
	if (current==NULL) {
	return 0;
	}
	if (! strcmp(move, current->contents)) {
	return 1;
	}
	current = current->next;
	}
}

//listcons initializer
listcons new_listcons(char*contents, listcons next) {
  listcons result = (listcons) malloc(sizeof(listcons_store));
  assert(result != NULL);
  result->contents = contents;
  result->next = next;
  return result;
}

//list initializer
list new_list() {
  list result = (list) malloc(sizeof(list_store));
  assert(result != NULL);
  result->front = NULL;
  return result;
}

//put string into list
void addtolist(char*thing,list stuff) {
  stuff->front= new_listcons(thing,stuff->front);
}


