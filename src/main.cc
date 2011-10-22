#include "CSITerminal.hh"
#include "RenderableTerminal.hh"

#include <iostream>

#include "boothby.hh"

int main ( int argc, char ** argv ) {
	init_screen();
	try {
		Terminal * t = new CSITerminal(80, 25);
		RenderableTerminal rt(t);
		
		Terminal * t2 = new CSITerminal(80, 25);
		RenderableTerminal rt2(t2);
		
		rt2.move_to(10,0);
		
		update_screen();
		t->fork("bash");
		t2->fork("./color.sh");
		
		while ( true ) {
			t->poke();
			t2->poke();
			
			rt.render();
			rt2.render();
			
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
