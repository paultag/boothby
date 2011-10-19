#include "Terminal.hh"
#include "RenderableTerminal.hh"

#include "boothby.hh"

int main ( int argc, char ** argv ) {
	init_screen();

	Terminal * t = new Terminal(80,25);
	RenderableTerminal rt(t);

	update_screen();

	char ch;

	while (( ch = getch() ) && (ch != 'q')) { 
		rt.insert(ch);
		rt.render();
		update_screen();
	}

	uninit_screen();
}
