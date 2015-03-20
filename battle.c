#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ncurses.h>
#include "rogue.h"

int monster_attack(amove m, monster p, hero h) {
  int roll1 = rand()%100;
  int roll2 = rand()%20;
  if (roll1<(m->tohit)) {
    int damage = ((m->power*(80+roll2)*(p->atk))/(h->def))/100;
    h->curhp -= damage;
    return damage;
  } else {
    return -1;
  }
}

int player_attack(amove m, monster p, hero h) {
  int roll1 = rand()%100;
  int roll2 = rand()%20;
  if (roll1<m->tohit) {
    int damage = ((m->power*(80+roll2)*(h->atk))/(p->def))/100;
    p->curhp -= damage;
    return damage;
  } else {
    return -1;
  }
}

//Graphics


int battle(monster p, hero h,int movefirst, moves movelist, WINDOW*topwin) {
  wclear(topwin);
  destroy_win(topwin);
  topwin=create_newwin(5,COLS,0,0);
  mvwprintw(topwin,1,1,"You entered battle with a level %d %s.",p->level,p->poketype->name);
  curs_set(1);
  
  keypad(topwin,TRUE);
  wrefresh(topwin);
  wgetch(topwin);
  if (movefirst) {
    wclear(topwin);
    destroy_win(topwin);
    topwin=create_newwin(5,COLS,0,0);
    mvwprintw(topwin,1,1,"You move first!");
    wrefresh(topwin);
    wgetch(topwin);
  } else {
    wclear(topwin);
    destroy_win(topwin);
    topwin=create_newwin(5,COLS,0,0);
    mvwprintw(topwin,1,1,"The monster moves first.");
    wrefresh(topwin);
    wgetch(topwin);
  }
  wrefresh(topwin);
  int curmove = movefirst;
  char input[100]; //Add hp + status stuff. 
  while (1) {
    if (curmove) {
      curmove=0;
      wclear(topwin);
      destroy_win(topwin);
      topwin=create_newwin(5,COLS,0,0);
      mvwprintw(topwin,1,1,"What move do you want to use? (l for list) ");
      wrefresh(topwin);
      echo();
      wscanw(topwin, "%s",&input);
      noecho();
      
      if (! strcmp(input, "l")) {
	//Show list of moves
      } else if (1) {
	amove m = find_move(input, movelist);
	int damage = player_attack(m,p,h);
	if (damage == -1) {
      wclear(topwin);
      destroy_win(topwin);
      topwin=create_newwin(5,COLS,0,0);
	  mvwprintw(topwin,1,1,"%s missed. You cry a little on the inside.\n", input);
      wrefresh(topwin);
      wgetch(topwin);
	} else if (damage > -1) {
      wclear(topwin);
      destroy_win(topwin);
      topwin=create_newwin(5,COLS,0,0);
	  mvwprintw(topwin,1,1, "%s dealt %d damage.\n", input, damage);
      wrefresh(topwin);
      wgetch(topwin);
	}
      }
    } else {
      curmove=1;
      int nummoves = leng(p->poketype->movelist);
      int ran=rand()%nummoves;
      amove m=find_move(p->poketype->movelist[ran],movelist);
      int damage=monster_attack(m,p,h);
      if (damage == -1) {
    wclear(topwin);
    destroy_win(topwin);
    topwin=create_newwin(5,COLS,0,0);
	mvwprintw(topwin,1,1,"Haha, the %s's move %s missed.\n", p->poketype->name, m->name);
    wrefresh(topwin);
    wgetch(topwin);
      } else if (damage < -1) {
    wclear(topwin);
    destroy_win(topwin);
    topwin=create_newwin(5,COLS,0,0);
	mvwprintw(topwin,1,1,"%s did %d damage to you with %s.\n", p->poketype->name, damage, m->name);
    wrefresh(topwin);
    wgetch(topwin);
      }
    }
    if (p->curhp<1) {
      curs_set(0);
      keypad(topwin,FALSE);
      return 0;
    }
    if (h->curhp<1) {
      curs_set(0);
      keypad(topwin,FALSE);
      return -1;
    }
  }
  
}

