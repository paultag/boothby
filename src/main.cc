#include "Terminal.hh"

int main ( int argc, char ** argv ) {
	Terminal t(80, 24);
	
	for ( int i = 0; i < ( 137 ); ++i )
		t.insert(65 + ( i % 10 ));

	initscr();

	t.render(stdscr);

	refresh();
	getch();
	endwin();
}
