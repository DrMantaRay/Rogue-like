#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>                  /*  for sleep()  */
#include <curses.h>


int main(void) {

    WINDOW * mainwin;

    
    /*  Initialize ncurses  */

    if ( (mainwin = initscr()) == NULL ) {
	fprintf(stderr, "Error initialising ncurses.\n");
	exit(EXIT_FAILURE);
    }


    /*  Display "Hello, world!" in the centre of the
	screen, call refresh() to show our changes, and
	sleep() for a few seconds to get the full screen effect  */

    mvaddstr(13, 33, "Welcome to Rogue!");
    refresh();
    sleep(3);
    erase();

    char* NAME;
    NAME = (char*) malloc(100);
    mvaddstr(13,33, "Enter a name: ");
    refresh();
    getstr(NAME);
    erase();
    refresh();
    move(0,0);
    for (int i=0; i<10; i++) {
      for (int j=0; j<10; j++) {
	move(i,j);
	addch('.');
      }
    }
    refresh();
    sleep(5);
    /*  Clean up after ourselves  */

    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}
