#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include "rogue.h"
#include <unistd.h>

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
int battle(WINDOW*westwin, monster p, hero h,int movefirst, list yourmovelist, moves movelist, WINDOW*topwin) {
  wclear(topwin);
  destroy_win(topwin);
  topwin=create_newwin(5,68,0,0);
  mvwprintw(topwin,1,1,"You entered battle with a level %d %s.",p->level,p->poketype->name);
  curs_set(1);
  keypad(topwin,TRUE);
  wrefresh(topwin);
  wgetch(topwin);
  if (movefirst) {
    wclear(topwin);
    destroy_win(topwin);
    topwin=create_newwin(5,68,0,0);
    mvwprintw(topwin,1,1,"You move first!");
    wrefresh(topwin);
    wgetch(topwin);
  } else {
    wclear(topwin);
    destroy_win(topwin);
    topwin=create_newwin(5,68,0,0);
    mvwprintw(topwin,1,1,"The monster moves first.");
    wrefresh(topwin);
    wgetch(topwin);
  }
  wrefresh(topwin);
  int curmove = movefirst;
  char input[100]; //Add hp + status stuff. 
  while (1) {
    wclear(westwin);
    wprintw(westwin,"\nHP:%d\n",h->curhp);
    wprintw(westwin,"LvL:%d\n", h->level);
    wprintw(westwin,"Exp:%d\n",h->exp);
    wrefresh(westwin);
    if (curmove) {
      curmove=0;
      int loopvar=1;
      while (loopvar) {
	wclear(topwin);
	destroy_win(topwin);
	topwin=create_newwin(5,68,0,0);
	mvwprintw(topwin,1,1,"What move do you want to use? (l for list) ");
	wrefresh(topwin);
	echo();
	wscanw(topwin, "%s",&input);
	noecho();
	if (! strcmp(input, "l")) {
	  listcons current = yourmovelist->front;
	  while(current != NULL) {
	    wclear(topwin);
	    destroy_win(topwin);
	    topwin=create_newwin(5,68,0,0);
	    mvwprintw(topwin,1,1,"%s (press any key to see next move)",current->contents);
        current=current->next;
	    wrefresh(topwin);
	    wgetch(topwin);
	  }
	  if (current == NULL) {
	    wclear(topwin);
	    destroy_win(topwin);
	    topwin=create_newwin(5,68,0,0);
	    mvwprintw(topwin,1,1,"End of movelist: returning to battle");
	    wrefresh(topwin);
	    sleep(2);
	  }
	} else if (is_move(input,yourmovelist)) {
        if (! strcmp("dance",input)) {
          if (strcmp(p->poketype->name,"time")) {
          wclear(topwin);
          destroy_win(topwin);
          topwin=create_newwin(5,68,0,0);
          mvwprintw(topwin,1,1,"You dance around in a frenzied jig. The %s doesn't seem amused.");
          wrefresh(topwin);
          wgetch(topwin);
          p->atk +=20;
          } else { 
          wclear(topwin);
          destroy_win(topwin);
          topwin=create_newwin(5,68,0,0);
          mvwprintw(topwin,1,1,"Your dancing transcends time itself!");
          wrefresh(topwin);
          wgetch(topwin);
          h->exp+=40*p->level;
          return 0;
          }
         }

	  loopvar = 0;
	  amove m = find_move(input, movelist);
	  if (strcmp(input,"heal")) {
	    int damage = player_attack(m,p,h);
	    if (damage == -1) {
	      wclear(topwin);
	      destroy_win(topwin);
	      topwin=create_newwin(5,68,0,0);
	      int choice = rand()%2;
	      if (choice==1) {
		mvwprintw(topwin,1,1,"%s missed. You cry a little on the inside.\n", input);
	      } else {
		mvwprintw(topwin,1,1,"The sneaky %s dodged your attack!", p->poketype->name);
	      }
	      wrefresh(topwin);
	      wgetch(topwin);
	    } else if (damage > -1) {
	      wclear(topwin);
	      destroy_win(topwin);
	      topwin=create_newwin(5,68,0,0);
	      mvwprintw(topwin,1,1, "%s dealt %d damage.\n", input, damage);
	      wrefresh(topwin);
	      wgetch(topwin);
	    }
	  } else {
	    int heal = m->power*h->level;
	    h->curhp += heal;
	    if (h->curhp > h->maxhp) {
	      h->curhp=h->maxhp;
	    }
	    wclear(topwin);
	    destroy_win(topwin);
	    topwin=create_newwin(5,68,0,0);
	    mvwprintw(topwin,1,1, "You healed for %d.\n",heal);
	    wrefresh(topwin);
	    wgetch(topwin);
	  }
	}
      }
    }
    else {
      curmove=1;
      int nummoves = leng(p->poketype->movelist);
      int ran=rand()%nummoves;
      amove m=find_move(p->poketype->movelist[ran],movelist);
      int damage=monster_attack(m,p,h);
      if (damage == -1) {
	wclear(topwin);
	destroy_win(topwin);
	topwin=create_newwin(5,68,0,0);
	mvwprintw(topwin,1,1,"Haha, the %s's move %s missed.\n", p->poketype->name, m->name);
	wrefresh(topwin);
	wgetch(topwin);
      } else if (damage > -1) {
	wclear(topwin);
	destroy_win(topwin);
	topwin=create_newwin(5,68,0,0);
	mvwprintw(topwin,1,1,"%s did %d damage to you with %s.\n", p->poketype->name, damage, m->name);
	wrefresh(topwin);
	wgetch(topwin);
      }
    }
    if (p->curhp<1) {
      h->exp += p->level;
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

