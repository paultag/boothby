#include <panel.h>

#include "Pane.hh"


Pane::Pane(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->height = height;
	this->width  = width;

	this->win = newwin(height, width, y, x);
	this->pan = new_panel(this->win);
	this->draw_outline();
}

void Pane::draw_outline() {
	box(this->win, 0, 0);
}

void Pane::focus() {
	top_panel(this->pan);
}

void Pane::move_to(int x, int y) {
	move_panel(this->pan, x, y);
}

void Pane::resize( int width, int height ) {
	WINDOW * old_win  = this->win;
	WINDOW * temp_win = newwin(height, width, this->y, this->x);

	replace_panel(this->pan, temp_win);
	this->win = temp_win;
	delwin(old_win);

	this->x = width;
	this->y = height;
	this->draw_outline();
}

WINDOW * Pane::getWindow() {
	return this->win;
}
