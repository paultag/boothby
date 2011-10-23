#include "CSITerminal.hh"
#include "RenderableTerminal.hh"

#include <iostream>

#include "boothby.hh"

int main ( int argc, char ** argv ) {
	init_screen();
	try {
		Terminal * t = new CSITerminal(80, 25);
		RenderableTerminal rt(t);
		rt.move_to(1, 1);
		
		update_screen();
		t->fork("bash");
		
		while ( true ) {
			t->poke();
			
			rt.render();
			
			update_screen();
			timeout(0);
			char ch = getch();
			if ( ch != ERR )
				t->type(ch);
			usleep(2000);
		}
		uninit_screen();
	} catch ( int i ) {
		uninit_screen();
		std::cout << "ERR: " << i << std::endl;
	}
	
}
