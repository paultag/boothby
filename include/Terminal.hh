#ifndef _TERMINAL_HH_
#define _TERMINAL_HH_ foo

#include <ncurses.h>

typedef struct _TerminalCell {
	unsigned char attr;
	unsigned char ch;
} TerminalCell;

class Terminal {
	protected:
		int width;
		int height;

		int cX;
		int cY;
		
		unsigned char cMode;
		TerminalCell * chars;

		bool handle_special_char( char c );
		void advance_curs();

	public:
		Terminal(int width, int height);
		void insert( char c );
		void scrollUp();
		void render( WINDOW * win );

		int getWidth();
		int getHeight();
		
};

#endif
