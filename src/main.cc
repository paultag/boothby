#include "Terminal.hh"
#include "RenderableTerminal.hh"

#include "boothby.hh"

int main ( int argc, char ** argv ) {
	init_screen();

	Terminal * t = new Terminal(80,25);
	RenderableTerminal rt(t);

	update_screen();

	int ch;

	while (( ch = getch() ) && (ch != 'q')) { 
		// real translation, please

		if ( ch == 127 )
			ch = '\b'; // this can't be right

		rt.insert(ch);
		rt.render();
		update_screen();
	}

	uninit_screen();
}
