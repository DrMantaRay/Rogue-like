#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ncurses.h>
#include "rogue.h"
#include <time.h>

int main(){
    FILE*movedata = fopen("movedata.txt","r");
    moves movelist = read_moves(movedata);
    fclose(movedata);
    FILE*pokedata = fopen("pokemondata.txt","r");
    pokemons pokelist = read_pokemons(pokedata);
    fclose(pokedata);
    monster amonster=init_monster(3,"gyrados",pokelist);
    hero ahero=new_hero(1,12,12,5,5);
    int startx,starty,row,col;
    int ch;
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    srand(time(NULL));
    if (LINES<24) {
        printw("Terminal window is too small(80x24 min)\n");
        getch();
        exit(1);
    }
    if (COLS<80) {
        printw("Terminal window is too small(80x24 min)\n");
        getch();
        exit(1);
    }
    clear();
    int sstartx=COLS/2;
    int sstarty=LINES/2;
    char name[100];
    printw("Enter your name: ");
    refresh();
    echo();
    scanw("%s\n", &name);
    noecho();
    clear();
    printw("Every leprachaun, at their coming of age, must find a rainbow as\n");
    printw("a coming of age ritual.");
    getch();
    clear();
    refresh();
    printw("%s the leprachaun was the most unlucky leprachaun to ever lived\n",name);
    printw("He had failed the test a record 2531 times.\n");
    printw("Press any key to continue.");
    getch();
    clear();
    refresh();
    curs_set(0);
    WINDOW*topwin;
    topwin=create_newwin(5,COLS,0,0);
    mvwprintw(topwin,1,1,"Press F2 twice to exit. Arrow keys for movement.");
    wrefresh(topwin);
    int i;
    int j;
    init_pair(1,COLOR_YELLOW,COLOR_RED);
    init_pair(3,COLOR_RED,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    init_pair(4,COLOR_BLUE,COLOR_BLACK);
    init_pair(5,COLOR_WHITE,COLOR_BLACK);
    WINDOW*botwin;
    botwin=create_newwin(19,COLS,5,0);
    int maxxbot,maxybot;
    getmaxyx(botwin,maxybot,maxxbot);
    startx=maxxbot/2;
    starty=maxybot/2;
    int flr=1;
    while (TRUE) {
        room aroom=map_initialize(flr,pokelist);
        flr++;
        wclear(botwin);
        display_room(botwin,startx,starty,aroom);
        wrefresh(botwin);
        mvwaddch(botwin,starty,startx,64 |A_BOLD|COLOR_PAIR(1));
        wrefresh(botwin);
        keypad(botwin,TRUE);
        char str;
        int x;
        int y;
        int ladder=0;
        while(((ch=wgetch(botwin)) !=KEY_F(2)) && !ladder) {
            switch(ch) {
                case KEY_RIGHT:
                    str=(mvwinch(botwin,starty,startx+1) & A_CHARTEXT);
                    if (str=='#') {
                        break;
                    }
                    if (mover(str,topwin,botwin,aroom->east,1,&x,&y)==1) {
                        aroom=aroom->east;
                        startx=x;
                        starty=y;
                        break;
                    }
                    if (str=='^') {
                        ladder=laddercheck(topwin,botwin);
                        break;
                    }
                    if (str=='U') {
                        monster mons=get_tile(botwin,aroom,startx+1,starty)->mons;
                        keypad(botwin,FALSE);
                        battle(mons, ahero,1, movelist, topwin);
                        keypad(botwin,TRUE);
                        break;
                    }
                    mvwaddch(botwin,starty,startx,'.'|aroom->cold);
                    mvwaddch(botwin,starty,++startx,64|A_BOLD|COLOR_PAIR(1));
                    break;
                case KEY_LEFT:
                    str=(mvwinch(botwin,starty,startx-1) & A_CHARTEXT);
                    if (str=='#') {
                        break;
                    }
                    if (str=='^') {
                        ladder=laddercheck(topwin,botwin);
                        break;
                    }
                    if (str=='U') {
                        monster mons=get_tile(botwin,aroom,startx-1,starty)->mons;
                        keypad(botwin,FALSE);
                        battle(mons, ahero,1, movelist, topwin);
                        keypad(botwin,TRUE);
                        break;
                    }
                    if (mover(str,topwin,botwin,aroom->west,3,&x,&y)==1) {
                        aroom=aroom->west;
                        startx=x;
                        starty=y;
                        break;
                    }
                    mvwaddch(botwin,starty,startx,'.'|aroom->cold);
                    mvwaddch(botwin,starty,--startx,64|A_BOLD|COLOR_PAIR(1));
                    break;
                case KEY_DOWN:
                    str=(mvwinch(botwin,starty+1,startx) & A_CHARTEXT);
                    if (str=='#') {
                        break;
                    }
                    if (str=='^') {
                        ladder=laddercheck(topwin,botwin);
                        break;
                    }
                    if (str=='U') {
                        monster mons=get_tile(botwin,aroom,startx,starty+1)->mons;
                        keypad(botwin,FALSE);
                        battle(mons, ahero,1, movelist, topwin);
                        keypad(botwin,TRUE);
                        break;
                    }
                    if (mover(str,topwin,botwin,aroom->south,2,&x,&y)==1) {
                        aroom=aroom->south;
                        startx=x;
                        starty=y;
                        break;
                    }
                    mvwaddch(botwin,starty,startx,'.'|aroom->cold);
                    mvwaddch(botwin,++starty,startx,64|A_BOLD|COLOR_PAIR(1));
                    break;
                case KEY_UP:
                    str=(mvwinch(botwin,starty-1,startx) & A_CHARTEXT);
                    if (str=='#') {
                        break;
                    }
                    if (str=='^') {
                        ladder=laddercheck(topwin,botwin);
                        break;
                    }
                    if (str=='U') {
                        monster mons=get_tile(botwin,aroom,startx,starty-1)->mons;
                        keypad(botwin,FALSE);
                        battle(mons, ahero,1, movelist, topwin);
                        keypad(botwin,TRUE);
                        break;
                    }
                    if (mover(str,topwin,botwin,aroom->north,0,&x,&y)==1) {
                        aroom=aroom->north;
                        startx=x;
                        starty=y;
                        break;
                    }
                    mvwaddch(botwin,starty,startx,'.'|aroom->cold);
                    mvwaddch(botwin,--starty,startx,64|A_BOLD|COLOR_PAIR(1));
                    break;
                default:
                    break;
            }
        }
        if (ch==KEY_F(2)) {
            break;
        }
        
    }
    endwin();			/* End curses mode		  */
    return 0;
}
