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

		bool handle_special_char( unsigned char c );
		
		bool handle_escape_char(  unsigned char c );
		bool handle_graph_char(   unsigned char c );
		bool handle_control_char( unsigned char c );

		void csi_atize();
		
		void advance_curs();

	public:
		Terminal(int width, int height);
		~Terminal();
		void insert( unsigned char c );
		void type( char c );
		void scroll_up();
		void render( WINDOW * win );
		void poke();
		pid_t fork( const char * command ); /* XXX: Protect this */

		int get_width();
		int get_height();
		
};

#endif
