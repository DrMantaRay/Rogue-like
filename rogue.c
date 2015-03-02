#include <stdio.h>
#include "graphics.h"
#include <curses.h>

int main()
{
    int gd= DETECT. gm;
    
    initgraph(&gd, &gm, "C:\\TC\\BGI");
    
    getch();
    closegraph();
    return 0;
}
