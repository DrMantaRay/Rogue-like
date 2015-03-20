#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ncurses.h>
#include "rogue.h"
#include <time.h>
#include <ctype.h>

int main(){
    FILE*movedata = fopen("movedata.txt","r");
    moves movelist = read_moves(movedata);
    fclose(movedata);
    FILE*pokedata = fopen("pokemondata.txt","r");
    pokemons pokelist = read_pokemons(pokedata);
    fclose(pokedata);
    hero ahero=new_hero(1,300,300,100,100);
    list yourmovelist=new_list();
    addtolist("heal", yourmovelist);
    addtolist("smoke_pipe", yourmovelist);
    addtolist("punch", yourmovelist);
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
    topwin=create_newwin(5,68,0,0);
    mvwprintw(topwin,1,1,"Press F2 to exit. Arrow keys for movement.");
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
    WINDOW*westwin;
    westwin=create_newwin(5,10,0,69);
    wrefresh(westwin);
    wbkgd(westwin,COLOR_PAIR(3));
    wclear(westwin);
    wprintw(westwin,"\nHP:%d\n",ahero->curhp);
    wprintw(westwin,"LvL:%d\n", ahero->level);
    wprintw(westwin,"Exp:%d\n",ahero->exp);
    wrefresh(westwin);
    while (TRUE) {
        room aroom=map_initialize(flr,pokelist);
        startx=maxxbot/2;
        starty=maxybot/2;
        flr++;
        wclear(botwin);
        display_room(botwin,startx,starty,aroom);
        mvwaddch(botwin,starty,startx,64 |A_BOLD|COLOR_PAIR(1));
        wrefresh(botwin);
        keypad(botwin,TRUE);
        char str;
        int x;
        int y;
        int ladder=0;
        tile *ntile= malloc(8*sizeof(tile));
        tile ctile;
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
                    if (str=='$') {
                        tile t=get_tile(botwin,aroom,startx+1,starty);
                        item it= get_tile(botwin,aroom,startx+1,starty)->anitem;
                        ahero->atk+=it->atk;
                        ahero->def+=it->def;
                        ahero->exp+=it->exp;
                        ahero->curhp+=it->curhp;
                        ahero->maxhp+=it->maxhp;
                        mvwaddch(botwin,starty,startx+1,'.'|aroom->cold);
                        for (i=0;i<aroom->xsize*aroom->ysize;++i) {
                            if (t->xcor==aroom->tilearray[i]->xcor) {
                                if (t->ycor==aroom->tilearray[i]->ycor) {
                                    aroom->tilearray[i]->anitem=NULL;
                                }
                            }
                        }
                        display_room(botwin,maxxbot/2,maxybot/2,aroom);       
                        mvwaddch(botwin,starty,startx,64 |A_BOLD|COLOR_PAIR(1)); 
                        break;
                    }
                    if (str=='^') {
                        ladder=laddercheck(topwin,botwin);
                        break;
                    }
                    if (isupper(str)) {
                        monster mons=get_tile(botwin,aroom,startx+1,starty)->mons;
                        tile t=get_tile(botwin,aroom,startx+1,starty);
                        keypad(botwin,FALSE);
                        if (battle(westwin,mons, ahero,1, yourmovelist,movelist, topwin)==-1) {
                            keypad(topwin,TRUE);
                            wclear(topwin);
                            destroy_win(topwin);
                            topwin=create_newwin(5,68,0,0);
                            mvwprintw(topwin,1,1,"You Die. How Sad.");
                            wrefresh(topwin);
                            wgetch(topwin);
                            clear();
                            echo();
                            exit(0);
                        }             
                        mvwaddch(botwin,starty,startx+1,'.'|aroom->cold);
                        for (i=0;i<aroom->xsize*aroom->ysize;++i) {
                            if (t->xcor==aroom->tilearray[i]->xcor) {
                                if (t->ycor==aroom->tilearray[i]->ycor) {
                                    aroom->tilearray[i]->mons=NULL;
                                }
                            }
                        }
                        display_room(botwin,maxxbot/2,maxybot/2,aroom);       
                        mvwaddch(botwin,starty,startx,64 |A_BOLD|COLOR_PAIR(1));        
                        keypad(topwin,FALSE);
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
                    if (str=='$') {
                        tile t= get_tile(botwin,aroom,startx-1,starty);
                        item it= get_tile(botwin,aroom,startx-1,starty)->anitem;
                        ahero->atk+=it->atk;
                        ahero->def+=it->def;
                        ahero->exp+=it->exp;
                        ahero->curhp+=it->curhp;
                        ahero->maxhp+=it->maxhp;
                        mvwaddch(botwin,starty,startx-1,'.'|aroom->cold);
                        for (i=0;i<aroom->xsize*aroom->ysize;++i) {
                            if (t->xcor==aroom->tilearray[i]->xcor) {
                                if (t->ycor==aroom->tilearray[i]->ycor) {
                                    aroom->tilearray[i]->anitem=NULL;
                                }
                            }
                        }
                        display_room(botwin,maxxbot/2,maxybot/2,aroom);       
                        mvwaddch(botwin,starty,startx,64 |A_BOLD|COLOR_PAIR(1)); 
                        break;
                    }
                    if (isupper(str)) {
                        monster mons=get_tile(botwin,aroom,startx-1,starty)->mons;
                        tile t= get_tile(botwin,aroom,startx-1,starty);
                        keypad(botwin,FALSE);
                        keypad(botwin,FALSE);
                        if (battle(westwin,mons, ahero,1, yourmovelist,movelist, topwin)==-1) {
                            keypad(topwin,TRUE);
                            wclear(topwin);
                            destroy_win(topwin);
                            topwin=create_newwin(5,68,0,0);
                            mvwprintw(topwin,1,1,"You Die. How Sad.");
                            wrefresh(topwin);
                            wgetch(topwin);
                            clear();
                            echo();
                            exit(0);
                        }  
                        mvwaddch(botwin,starty,startx-1,'.'|aroom->cold);     
                        for (i=0;i<aroom->xsize*aroom->ysize;++i) {
                            if (t->xcor==aroom->tilearray[i]->xcor) {
                                if (t->ycor==aroom->tilearray[i]->ycor) {
                                    aroom->tilearray[i]->mons=NULL;
                                }
                            }
                        }
                        display_room(botwin,maxxbot/2,maxybot/2,aroom);     
                        mvwaddch(botwin,starty,startx,64 |A_BOLD|COLOR_PAIR(1));                     
                        keypad(topwin,FALSE);
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
                    if (str=='$') {
                        tile t= get_tile(botwin,aroom,startx,starty+1);
                        item it= get_tile(botwin,aroom,startx,starty+1)->anitem;
                        ahero->atk+=it->atk;
                        ahero->def+=it->def;
                        ahero->exp+=it->exp;
                        ahero->curhp+=it->curhp;
                        ahero->maxhp+=it->maxhp;
                        mvwaddch(botwin,starty+1,startx,'.'|aroom->cold);
                        for (i=0;i<aroom->xsize*aroom->ysize;++i) {
                            if (t->xcor==aroom->tilearray[i]->xcor) {
                                if (t->ycor==aroom->tilearray[i]->ycor) {
                                    aroom->tilearray[i]->anitem=NULL;
                                }
                            }
                        }
                        display_room(botwin,maxxbot/2,maxybot/2,aroom);       
                        mvwaddch(botwin,starty,startx,64 |A_BOLD|COLOR_PAIR(1)); 
                        break;
                    }
                    if (str=='^') {
                        ladder=laddercheck(topwin,botwin);
                        break;
                    }
                    if (isupper(str)) {
                        monster mons=get_tile(botwin,aroom,startx,starty+1)->mons;
                        tile t= get_tile(botwin,aroom,startx,starty+1);
                        keypad(botwin,FALSE);
                        keypad(botwin,FALSE);
                        if (battle(westwin,mons, ahero,1, yourmovelist,movelist, topwin)==-1) {
                            keypad(topwin,TRUE);
                            wclear(topwin);
                            destroy_win(topwin);
                            topwin=create_newwin(5,68,0,0);
                            mvwprintw(topwin,1,1,"You Die. How Sad.");
                            wrefresh(topwin);
                            wgetch(topwin);
                            clear();
                            echo();
                            exit(0);
                        }              
                        mvwaddch(botwin,starty+1,startx,'.'|aroom->cold);
                        for (i=0;i<aroom->xsize*aroom->ysize;++i) {
                            if (t->xcor==aroom->tilearray[i]->xcor) {
                                if (t->ycor==aroom->tilearray[i]->ycor) {
                                    aroom->tilearray[i]->mons=NULL;
                                }
                            }
                        }
                        display_room(botwin,maxxbot/2,maxybot/2,aroom);   
                        mvwaddch(botwin,starty,startx,64 |A_BOLD|COLOR_PAIR(1));           
                        keypad(topwin,FALSE);
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
                    if (str=='$') {
                         tile t=get_tile(botwin,aroom,startx,starty-1);
                        item it= get_tile(botwin,aroom,startx,starty-1)->anitem;
                        ahero->atk+=it->atk;
                        ahero->def+=it->def;
                        ahero->exp+=it->exp;
                        ahero->curhp+=it->curhp;
                        ahero->maxhp+=it->maxhp;
                        mvwaddch(botwin,starty-1,startx,'.'|aroom->cold);
                        for (i=0;i<aroom->xsize*aroom->ysize;++i) {
                            if (t->xcor==aroom->tilearray[i]->xcor) {
                                if (t->ycor==aroom->tilearray[i]->ycor) {
                                    aroom->tilearray[i]->anitem=NULL;
                                }
                            }
                        }
                        display_room(botwin,maxxbot/2,maxybot/2,aroom);       
                        mvwaddch(botwin,starty,startx,64 |A_BOLD|COLOR_PAIR(1)); 
                        break;
                    }
                    if (isupper(str)) {
                        monster mons=get_tile(botwin,aroom,startx,starty-1)->mons;
                        tile t=get_tile(botwin,aroom,startx,starty-1);
                        keypad(botwin,FALSE);
                        keypad(botwin,FALSE);
                        if (battle(westwin,mons, ahero,1, yourmovelist,movelist, topwin)==-1) {
                            keypad(topwin,TRUE);
                            wclear(topwin);
                            destroy_win(topwin);
                            topwin=create_newwin(5,68,0,0);
                            mvwprintw(topwin,1,1,"You Die. How Sad.");
                            wrefresh(topwin);
                            wgetch(topwin);
                            clear();
                            echo();
                            exit(0);
                        }  
                        mvwaddch(botwin,starty-1,startx,'.'|aroom->cold);  
                        for (i=0;i<aroom->xsize*aroom->ysize;++i) {
                            if (t->xcor==aroom->tilearray[i]->xcor) {
                                if (t->ycor==aroom->tilearray[i]->ycor) {
                                    aroom->tilearray[i]->mons=NULL;
                                }
                            }
                        }
                        display_room(botwin,maxxbot/2,maxybot/2,aroom);     
                        mvwaddch(botwin,starty,startx,64 |A_BOLD|COLOR_PAIR(1));            
                        keypad(topwin,FALSE);
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
            wclear(westwin);
            wprintw(westwin,"\nHP:%d\n",ahero->curhp);
            wprintw(westwin,"LvL:%d\n", ahero->level);
            wprintw(westwin,"Exp:%d\n",ahero->exp);
            wrefresh(westwin);
            int myx,myy;
            myx=startx-maxxbot/2+aroom->xsize/2;
            myy=starty-maxybot/2+aroom->ysize/2;
            for (i=0;i<aroom->xsize*aroom->ysize;i++) {
                int ran=rand()%8;   
                if (aroom->tilearray[i]->mons !=NULL) {
                    ntile[0]=get_local(aroom,aroom->tilearray[i]->xcor+1,aroom->tilearray[i]->ycor+1);
                    ntile[1]=get_local(aroom,aroom->tilearray[i]->xcor+1,aroom->tilearray[i]->ycor-1);
                    ntile[2]=get_local(aroom,aroom->tilearray[i]->xcor-1,aroom->tilearray[i]->ycor+1);
                    ntile[3]=get_local(aroom,aroom->tilearray[i]->xcor-1,aroom->tilearray[i]->ycor-1);
                    ntile[4]=get_local(aroom,aroom->tilearray[i]->xcor,aroom->tilearray[i]->ycor+1);
                    ntile[5]=get_local(aroom,aroom->tilearray[i]->xcor,aroom->tilearray[i]->ycor-1);
                    ntile[6]=get_local(aroom,aroom->tilearray[i]->xcor+1,aroom->tilearray[i]->ycor);
                    ntile[7]=get_local(aroom,aroom->tilearray[i]->xcor-1,aroom->tilearray[i]->ycor);
                    if (ntile[ran]->form=='.'&&ntile[ran]->mons==NULL) {
                        ntile[ran]->mons=aroom->tilearray[i]->mons;
                        aroom->tilearray[i]->mons=NULL;
                    }
                    if (ntile[ran]->xcor ==myx && ntile[ran]->ycor==myy) {
                        if (battle(westwin,ntile[ran]->mons, ahero,0, yourmovelist,movelist, topwin)==-1) {
                            keypad(topwin,TRUE);
                            wclear(topwin);
                            destroy_win(topwin);
                            topwin=create_newwin(5,68,0,0);
                            mvwprintw(topwin,1,1,"You Die. How Sad.");
                            wrefresh(topwin);
                            wgetch(topwin);
                            clear();
                            echo();
                            exit(0);
                        }
                        ntile[ran]->mons=NULL;
                    }                    
                }
            }
            display_room(botwin,maxxbot/2,maxybot/2,aroom);
            mvwaddch(botwin,starty,startx,64 |A_BOLD|COLOR_PAIR(1));
            if (ahero->exp > 40*ahero->level) {
                  ahero->exp = 0;
                  ahero->level++;
                  if (ahero->level==2) {
                    addtolist("dance",yourmovelist);
                  }
                  if (ahero->level==5) {
                    addtolist("fireball", yourmovelist);
                  }
                  if (ahero->level==20) {
                    addtolist("excalibur", yourmovelist);
                  }
                  int randstat1 = rand()%25;
                  int randstat2 = rand()%25;
                  int randstat3 = rand()%150;
                  ahero->atk += randstat1;
                  ahero->def += randstat2;
                  ahero->curhp += randstat3;
                  ahero->maxhp += randstat3;
                  wclear(topwin);
                  destroy_win(topwin);
                  topwin=create_newwin(5,68,0,0);
                  mvwprintw(topwin,1,1,"You leveled up. Grats. No, really, I mean it.");
                  wrefresh(topwin);
                  wgetch(topwin);
                 }

                if (ahero->curhp>ahero->maxhp) {
                  ahero->curhp = ahero->maxhp;
                 }
  
            
        }
        if (ch==KEY_F(2)) {
            break;
        }
        
    }
    endwin();			/* End curses mode		  */
    return 0;
}
