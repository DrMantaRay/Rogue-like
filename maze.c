#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "rogue.h"



tile get_tile(WINDOW* w, room rm, int x,int y) {
    int maxxbot,maxybot;
    getmaxyx(w,maxybot,maxxbot);
    int startx=maxxbot/2;
    int starty=maxybot/2;
    int i;
    int winx,winy;
    winx=x-startx+rm->xsize/2;
    winy=y-starty+rm->ysize/2;
    for (i=0;i<rm->xsize*rm->ysize;++i) {
        if (rm->tilearray[i]->xcor==winx &&rm->tilearray[i]->ycor==winy) {
            return rm->tilearray[i];
        }
    }
}
room map_initialize(int flr,pokemons pokelist) {
    int i;
    int j;
    init_pair(3,COLOR_RED,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    init_pair(4,COLOR_BLUE,COLOR_BLACK);
    init_pair(5,COLOR_WHITE,COLOR_BLACK);
    int numrooms=(rand()%5)+7;
    room*roomlist= (room*) calloc(numrooms,sizeof(room));
    for (i=0;i<numrooms;++i) {
        roomlist[i]=init_room(rand()%25+15,rand()%9+6,COLOR_PAIR(2),COLOR_PAIR(3),COLOR_PAIR(4),COLOR_PAIR(5));
    }
    int extra=(rand()%3)+2;
    room*extrarooms=(room*) calloc(extra,sizeof(room));
    for (i=0;i<extra;++i) {
        extrarooms[i]=init_room(rand()%25+15,rand()%9+6,COLOR_PAIR(2),COLOR_PAIR(3),COLOR_PAIR(4),COLOR_PAIR(5));
    }
    
    int rando=rand()%numrooms;
    roomlist[rando]->downroom=1;
    int roomwidth= rand()%(roomlist[rando]->xsize-2);
    int roomheight= rand()%(roomlist[rando]->ysize-2);

    for (j=0;j<roomlist[rando]->xsize*roomlist[rando]->ysize;j++) {
        if (roomlist[rando]->tilearray[j]->xcor == roomwidth+1) {
            if (roomlist[rando]->tilearray[j]->ycor== roomheight+1) {
                roomlist[rando]->tilearray[j]->item='^';
            }
        }
    }
    
    for (i=0;i<numrooms-1;++i) {
        room_match(roomlist[i],roomlist[i+1]);
    }
    for (i=0;i<extra;++i) {
        while (TRUE) {
            int att=rand()%numrooms;
            if (!room_match(roomlist[att],extrarooms[i])) {
                break;
            }
        }
    }
    for (i=0;i<numrooms;++i) {
        create_monsters(roomlist[i],flr,"dragonite",pokelist);
        create_portals(roomlist[i]);
    }
    
    for (i=0;i<extra;++i) {
        create_monsters(extrarooms[i],flr,"dragonite",pokelist);
        create_portals(extrarooms[i]);
    }
    return roomlist[0];
}
int mover(char ch,WINDOW*pwin,WINDOW*dwin,room rm,int side,int*x,int*y) {
    int maxybot,maxxbot;
    getmaxyx(dwin,maxybot,maxxbot);
    int startx=maxxbot/2;
    int starty=maxybot/2;
    int i;
    
    if (ch=='\\') {
        wclear(dwin);
        display_room(dwin,startx,starty,rm);
        int winx=startx-rm->xsize/2;
        int winy=starty-rm->ysize/2;
        if (side==0) {
            mvwaddch(dwin,winy+rm->sy-1,winx+rm->sx,64 |A_BOLD|COLOR_PAIR(1));
            *x=winx+rm->sx;
            *y=winy+rm->sy-1;
        }
        else if (side==1) {
            mvwaddch(dwin,winy+rm->wy,winx+rm->wx+1,64 |A_BOLD|COLOR_PAIR(1));
            *x=winx+rm->wx+1;
            *y=winy+rm->wy;
        }
        else if (side ==2) {
            mvwaddch(dwin,winy+rm->ny+1,winx+rm->nx,64 |A_BOLD|COLOR_PAIR(1));
            *x=winx+rm->nx;
            *y=winy+rm->ny+1;
        }
        else if (side==3) {
            mvwaddch(dwin,winy+rm->ey,winx+rm->ex-1,64 |A_BOLD|COLOR_PAIR(1));
            *x=winx+rm->ex-1;
            *y=winy+rm->ey;
            
        }
        return 1;
        
    }
    return 0;
}

int laddercheck(WINDOW*topwin,WINDOW*botwin) {
    char res;
    while (TRUE) {
        wclear(topwin);
        destroy_win(topwin);
        topwin=create_newwin(5,COLS,0,0);
        mvwprintw(topwin,1,1,"Would you like to go up a floor?(y/n)\n");
        curs_set(1);
        keypad(botwin,FALSE);
        keypad(topwin,TRUE);
        wrefresh(topwin);
        echo();
        mvwscanw(topwin,2,1,"%c\n", &res);
        noecho();
        curs_set(0);
        if (res=='y') {
            wclear(topwin);
            destroy_win(topwin);
            topwin=create_newwin(5,COLS,0,0);
            mvwprintw(topwin,1,1,"You climb up to the next floor.");
            wrefresh(topwin);
            keypad(topwin,FALSE);
            keypad(botwin,TRUE);
            wrefresh(botwin);
            return 1;
        }
        else if (res=='n') {
            keypad(topwin,FALSE);
            keypad(botwin,TRUE);
            wrefresh(botwin);
            return 0;
        }
    }   
}

room get_next(room aroom,int i) {
    room result;
    if (i==0) {
        result= aroom->north;
    }
    if (i==1) {
        result= aroom->east;
    }
    if (i==2) {
        result= aroom->south;
    }
    if (i==3) {
        result= aroom->west;
    }
    return result;
}
int room_match(room ra,room rb) {
    int i;
    int n=0;
    for (i=0;i<4;++i) {
        if (get_next(ra,i)!=NULL) {
            ++n;
        }
        if (n==3) {
            return 1;
        }
    }
    n=0;
    for (i=0;i<4;++i) {
        if (get_next(rb,i)!=NULL) {
            ++n;
        }
        if (n==3) {
            return 1;
        }
    }
    while (TRUE) {
        int a= rand()%4;
        int b= (a+2)%4;
        if (get_next(ra,a) ==NULL) {
            if (get_next(rb,b)==NULL) {
                if (a==0) {
                    ra->north=rb;
                    rb->south=ra;
                    return 0;
                }
                else if (a==1) {
                    ra->east=rb;
                    rb->west=ra;
                    return 0;
                }
                else if (a==2) {
                    ra->south=rb;
                    rb->north=ra;
                    return 0;
                }
                else if (a==3) {
                    ra->west=rb;
                    rb->east=ra;
                    return 0;
                }
            }
        }
    }
    return 1;
}

room init_room(int width,int height,int colw,int cold,int colladder,int coldoor) {
    room result= malloc(sizeof(struct room_store));
    result->xsize=width;
    result->ysize=height;
    result->finalroom=0;
    result->rainbowroom=0;
    result->downroom=0;
    result->colw=colw;
    result->cold=cold;
    result->colladder=colladder;
    result->coldoor=coldoor;
    int i;
    int j;
    int n=0;
    result->tilearray= (tile*) calloc(width*height,sizeof(tile));
    for (n=0;n<width*height;n++) {
        result->tilearray[n]= (tile) malloc(sizeof(struct tile_store));
    }
    n=0;
    for (i=1;i<width-1;++i) {
        for (j=1;j<height-1;++j) {
            result->tilearray[n]->xcor=i;
            result->tilearray[n]->ycor=j;
            result->tilearray[n]->item='.';
            n++;
            
        }
    }
   
    
    for (i=1;i<width-1;++i) {
        result->tilearray[n]->xcor=i;
        result->tilearray[n]->ycor=0;
        result->tilearray[n]->item='#';
        n++;
    }
    for (i=1;i<width-1;++i) {
        result->tilearray[n]->xcor=i;
        result->tilearray[n]->ycor=height-1;
        result->tilearray[n]->item='#';
        n++;
    }
    for (j=0;j<height;++j) {
        result->tilearray[n]->xcor=0;
        result->tilearray[n]->ycor=j;
        result->tilearray[n]->item='#';
        n++;
    }
    for (j=0;j<height;++j) {
        result->tilearray[n]->xcor=width-1;
        result->tilearray[n]->ycor=j;
        result->tilearray[n]->item='#';
        n++;
        if (n==width*height) {
            break;
        }
    }
    return result;
}

void free_room (room rm) {
    int i;
    for (i=0;i<rm->xsize*rm->ysize;++i) {
        free ((void *) rm->tilearray[i]);
    }
    free ((void*) rm->tilearray);
    free ((void*) rm);
}
void display_room(WINDOW *win, int startx,int starty,room aroom) {
    int n;
    int col;
    int maxx,minx,maxy,miny;
    int starti=startx-aroom->xsize/2;
    int startj=starty-aroom->ysize/2;
    for (n=0;n<aroom->xsize*aroom->ysize;++n){
        if (aroom->tilearray[n]->item=='#') {
            col=aroom->colw;
        }
        else if (aroom->tilearray[n]->item=='\\') {
            col=aroom->coldoor;
        }
        else if (aroom->tilearray[n]->item=='^') {
            col=aroom->colladder;
        }
        else {
            col=aroom->cold;
        }
            
        mvwaddch(win,startj+aroom->tilearray[n]->ycor,starti+aroom->tilearray[n]->xcor,aroom->tilearray[n]->item |col);
        if (aroom->tilearray[n]->mons != NULL) {
             mvwaddch(win,startj+aroom->tilearray[n]->ycor,starti+aroom->tilearray[n]->xcor,*aroom->tilearray[n]->mons->poketype->form |col);
        }
            
    }
    
}

