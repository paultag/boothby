#ifndef _PANE_HH_
#define _PANE_HH_ FOO

#include <panel.h>

class Pane {
	private:
		WINDOW * win;
		PANEL  * pan;

		int x;
		int y;
		int width;
		int height;

	public:
		Pane(int x, int y, int width, int height);
		~Pane();
		void focus();
		void move_to( int x, int y );
		void resize( int width, int height );
		void draw_outline();

		WINDOW * getWindow();
};

#endif
