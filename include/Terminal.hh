#ifndef _TERMINAL_HH_
#define _TERMINAL_HH_ foo

#include <ncurses.h>
#include <pty.h>

typedef struct _TerminalCell {
	unsigned char attr;
	unsigned char ch;
} TerminalCell;

class Terminal {
	protected:

	/* iVars */

		bool graph;
		bool special;

		int width;
		int height;

		int cX;
		int cY;
		
		unsigned char   cMode;
		TerminalCell  * chars;

		char  *  escape;
		int      maxesc;

		pid_t  pty;
		pid_t  childpid;

	/* Functions */

		void advance_curs();
		void _init_Terminal( int width, int height );

	public:
		Terminal(int width, int height);
		Terminal();
		~Terminal();
		virtual void insert( unsigned char c );
		void type( char c );
		void scroll_up();
		void render( WINDOW * win );
		void poke();
		pid_t fork( const char * command ); /* XXX: Protect this? */

		int get_width();
		int get_height();
		
};

#endif
