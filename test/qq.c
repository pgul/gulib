#include <stdio.h>
#include <curses.h>

int main(void)
{
	initscr();
	cbreak();
	noecho();
	nonl();
	start_color();

	//move(10, 10);
	//init_pair(1, 6, 1);
	//attr_set(COLOR_PAIR(1) | A_BOLD, 1, NULL);
	//attrset(COLOR_PAIR(1) | A_BOLD);
	//attron(COLOR_PAIR(1) | A_BOLD);
	//attr_on(COLOR_PAIR(1) | A_BOLD, NULL);
	//color_set(1, NULL);
#if 0
	addch(0x90);
	addch(0x91);
	addch(0x92);
	refresh();
#else
	move(10, 10);
	refresh();
	putp(tparm(tigetstr("setaf"), 1));
	putchar(0x90);
	putchar(0x91);
	putchar(0x92);
	fflush(stdout);
#endif

	getch();
	endwin();
	return 0;
}
