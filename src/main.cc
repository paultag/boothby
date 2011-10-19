#include "Terminal.hh"
#include "RenderableTerminal.hh"

#include <iostream>

#include "boothby.hh"

int main ( int argc, char ** argv ) {
	init_screen();

	/*Terminal * t = new Terminal(80, 25);
	RenderableTerminal rt(t);
	update_screen();
	t->fork("bash");
	while ( true ) {
		t->poke();
		rt.render();
		update_screen();
		usleep(2000);
		timeout(0);
		char ch = getch();
		if ( ch != ERR )
			t->type(ch);
	} */

	uninit_screen();
}
