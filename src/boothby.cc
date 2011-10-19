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

int get_screen_x() { //XXX: Clean this up
	int row, col;
	getmaxyx(stdscr,row,col);
	return col;
}

int get_screen_y() { //XXX: Clean this up
	int row, col;
	getmaxyx(stdscr,row,col);
	return row;
}
