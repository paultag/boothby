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

	protected:
		void draw_outline();

	public:
		Pane(int x, int y, int width, int height);
		void focus();
		void move_to( int x, int y );
		void resize( int width, int height );
};

#endif
