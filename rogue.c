#include <stdio.h>
#include <ncurses.h>
#include "rogue.h"
#include <stdlib.h>

int main(){
    int startx,starty,row,col;
    int ch;
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    if (LINES<24) {
        printw("Terminal window is too small(80x24 min)\n");
        exit(1);
    }
    if (COLS<80) {
        printw("Terminal window is too small(80x24 min)\n");
        exit(1);
    }
    startx=COLS/2;
    starty=LINES/2;
    printw("Every leprachaun, at their coming of age, must find a rainbow as\n");
    printw("a coming of age ritual\n");
    printw("Saul the leprachaun was the most unlucky leprachaun to ever lived\n");
    printw("He had failed the test a record 2531 times\n");
    printw("Press any key to continue");
    getch();
    refresh();
    int i;
    int j;
    int maxx,minx,maxy,miny;
    maxx=startx+39;
    minx=startx-39;
    maxy=starty+11;
    miny=starty-11;
    init_pair(2,COLOR_BLUE,COLOR_BLACK);
    clear();
    for (i=miny;i<maxy;i++) {
        for (j=minx;j<maxx;j++) {
            mvaddch(i,j,'.'|COLOR_PAIR(2));
        }
    }
    for (j=minx;j<maxx;j++) {
            mvaddch(miny-1,j,'-');
    }
    for (j=minx;j<maxx;j++) {
            mvaddch(maxy,j,'-');
    }
    for (i=miny;i<maxy;i++) {
            mvaddch(i,maxx,'|');
    }
    for (i=miny;i<maxy;i++) {
            mvaddch(i,minx-1,'|');
    }
    
    refresh();
    init_pair(1,COLOR_YELLOW,COLOR_RED);
    mvaddch(starty,startx,64 |A_BOLD|COLOR_PAIR(1));
    while((ch=getch()) !=KEY_F(2)) {
        switch(ch) {
            case KEY_RIGHT:
                if (startx==maxx-1) {
                    break;
                }
                mvaddch(starty,startx,'.'|COLOR_PAIR(2));
                mvaddch(starty,++startx,64|A_BOLD|COLOR_PAIR(1));
                break;
            case KEY_LEFT:
                if (startx==minx) {
                    break;
                }
                mvaddch(starty,startx,'.'|COLOR_PAIR(2));
                mvaddch(starty,--startx,64|A_BOLD|COLOR_PAIR(1));
                break;
            case KEY_DOWN:
                if (starty==maxy-1) {
                    break;
                }
                mvaddch(starty,startx,'.'|COLOR_PAIR(2));
                mvaddch(++starty,startx,64|A_BOLD|COLOR_PAIR(1));
                break;
            case KEY_UP:
                if (starty==miny) {
                    break;
                }
                mvaddch(starty,startx,'.'|COLOR_PAIR(2));
                mvaddch(--starty,startx,64|A_BOLD|COLOR_PAIR(1));
                break;
            default:
                break;
        }
    }
        
    refresh();
    getch();
	endwin();			/* End curses mode		  */
	return 0;
}
