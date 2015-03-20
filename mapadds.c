#include <stdio.h>
#include <ncurses.h>
#include "rogue.h"
#include <stdlib.h>
#include <time.h>

//creates items given a room.
void create_items(room rm,int flr) {
    int i;
    int r;
    for (i=0;i<rm->xsize*rm->ysize;++i) {
        if (rm->tilearray[i]->form=='.') {
            r=rand()%200;
            if (r==0) {
                rm->tilearray[i]->anitem=init_item(flr);
                rm->tilearray[i]->anitem->form='$';
            }
        }
    }
}
//creates monsters given a room.
void create_monsters(room rm,int flr, char* pokemon, pokemons pokelist) {
    int i;
    int r;
    for (i=0;i<rm->xsize*rm->ysize;++i) {
        if (rm->tilearray[i]->form == '.' && rm->tilearray[i]->mons==NULL) {
            r=rand()%150;
            if (r==0) {
                rm->tilearray[i]->mons=init_monster(flr,pokemon,pokelist);                
            }
            else {
                rm->tilearray[i]->mons=NULL;
            }
        }
    }
}
//creates portals given a room. terrible hack for different portal directions.
int create_portals(room ar) {
    int ran;
    int i;
    if (get_next(ar,0)!=NULL) {
        ran=rand()% (ar->xsize-2) +1;
        for (i=0;i<ar->xsize*ar->ysize;++i) {
            if (ar->tilearray[i]->xcor==ran &&ar->tilearray[i]->ycor==0) {
                ar->tilearray[i]->form='\\';
                ar->tilearray[i]->aroom=ar->north;
                ar->nx=ar->tilearray[i]->xcor;
                ar->ny=ar->tilearray[i]->ycor;
            }
        }
    }
    if (get_next(ar,1)!=NULL) {
        ran=rand()% (ar->ysize-2)+1;
        for (i=0;i<ar->xsize*ar->ysize;++i) {
            if (ar->tilearray[i]->ycor==ran &&ar->tilearray[i]->xcor==ar->xsize-1) {
                ar->tilearray[i]->form='\\';
                ar->tilearray[i]->aroom=ar->east;
                ar->ex=ar->tilearray[i]->xcor;
                ar->ey=ar->tilearray[i]->ycor;
            }
        }
    }
    if (get_next(ar,2)!=NULL) {
        ran=rand()% (ar->xsize-2)+1;
        for (i=0;i<ar->xsize*ar->ysize;++i) {
            if (ar->tilearray[i]->xcor==ran &&ar->tilearray[i]->ycor==ar->ysize-1) {
                ar->tilearray[i]->form='\\';
                ar->tilearray[i]->aroom=ar->south;
                ar->sx=ar->tilearray[i]->xcor;
                ar->sy=ar->tilearray[i]->ycor;
            }
        }
    }
    if (get_next(ar,3)!=NULL) {
        ran=rand()% (ar->ysize-2)+1;
        for (i=0;i<ar->xsize*ar->ysize;++i) {
            if (ar->tilearray[i]->ycor==ran &&ar->tilearray[i]->xcor==0) {
                ar->tilearray[i]->form='\\';
                ar->tilearray[i]->aroom=ar->west;
                ar->wx=ar->tilearray[i]->xcor;
                ar->wy=ar->tilearray[i]->ycor;
            }
        }
    }
    return 1;
}
