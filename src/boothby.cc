#include <panel.h>
#include <ncurses.h>

#include "boothby.hh"

void init_screen() {
	initscr();
	cbreak();
	noecho();
}

void uninit_screen() {
	endwin();
}

void update_screen() {
	refresh();
	update_panels();
	doupdate();
}
