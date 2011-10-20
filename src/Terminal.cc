#include <malloc.h>
#include <iostream>
#include <pty.h>
#include <stdlib.h>
#include <string.h>

#include "Terminal.hh"

Terminal::Terminal(int width, int height) {
	this->graph  = false;
	this->width  = width;
	this->height = height;
	this->cX     = 0;
	this->cY     = 0;
	this->cMode  = 0x70; // 112

	this->pty    = -1;

	this->special   = false;
	this->maxesc    = 128; /* Hopefully more then we ever need. */
	this->escape    = (char *)malloc(sizeof(char) * this->maxesc);
	this->escape[0] = '\0';
	
	this->chars = (TerminalCell*)
		malloc(sizeof(TerminalCell) * (width * height));
 
	for ( int i = 0; i < ( height * width ); ++i ) {
		this->chars[i].ch   = ' ';
		this->chars[i].attr = this->cMode;
	}
}

void Terminal::scroll_up() {
	for ( int iy = 1; iy < this->height; iy++ ) {
		for ( int ix = 0; ix < this->width; ++ix ) {
			int thisChar = (( this->width *   iy      ) + ix );
			int lastChar = (( this->width * ( iy - 1 )) + ix );
			this->chars[lastChar].ch = this->chars[thisChar].ch;
		}
	}
	for ( int ix = 0; ix < this->width; ++ix ) {
		int offset = ((this->width * (this->height - 1)) + ix);
		this->chars[offset].ch = ' ';
	}
}

void Terminal::render( WINDOW * win ) {
	for ( int iy = 0; iy < this->height; ++iy ) {
		for ( int ix = 0; ix  < this->width; ++ix ) {
			int offset = (( this->width * iy ) + ix );
			mvwaddch(win, iy, ix, this->chars[offset].ch);
		}
	}
}

bool Terminal::handle_escape_char( char c ) {
	if ( ! this->special )
		return false;
	int idex = strlen(this->escape);
	
	if ( idex > this->maxesc )
		throw -2;
	
	this->escape[idex]   = c;
	this->escape[idex+1] = '\0';

	char f = this->escape[0];
	char l = this->escape[idex];

	bool is_l_csi = (
		( l >= 'a' && l <= 'z' ) ||
		( l >= 'A' && l <= 'Z' ) ||
		  l == '@'               ||
		  l == '`'
	);

	if ( f != '[' && f != ']' ) {
		this->escape[0] = '\0';
		this->special   = false;
		return true; // donezo.
	}

	if ( f == '[' && is_l_csi ) {
		this->special = false;
		// do things (CSI)
		this->escape[0] = '\0';
		return true;
	}
	
	if ( f == ']' && l == '\a' ) {
		this->special = false;
		// do xterm things
		this->escape[0] = '\0';
		return true;
	}
	
	return true;
}
bool Terminal::handle_graph_char( char c ) {
	
	/* This was yanked directly from rote */
	
	if ( ! this->graph )
		return false;
	
	char nc;
	switch (c) {
		case 'k': case 'l': case 'm': case 'n':
		case 't': case 'u': case 'v': case 'w':
		case 'j':
			nc = '+';
			break;
		case 'x':
			nc = '|';
			break;
		default:
			nc = '%';
			break;
	}

	this->insert(nc); // come back at it like a mofo.

	return true;
}
bool Terminal::handle_control_char( char c ) {
	switch ( c ) {
		case '\n': /* newline */
			this->cX = this->width; /* XXX: Fix this hack */
			this->advance_curs();
			return true;
			break;
		case '\b': /* backspace */
			if ( this->cX )
				this->cX--;
			return true;
			break;
		case '\t': /* tab */
			while (this->cX % 8)
				this->insert(' ');
			return true;
			break;
		case '\a': /* bell */
			return true;
			break;
		case 27:
			this->escape[0] = '\0';
			this->special   = true;
			return true;
			break;
		case 0x9B:
			this->escape[0] = '[';
			this->escape[1] = '\0';
			this->special   = true;
			break;
		default:
			break;
	}

	// some last ditch crap 
	if ( c < 32 )
		return true;

	return false;
}

bool Terminal::handle_special_char( char c ) {

	if ( this->handle_escape_char(c) )
		return true;
	else if ( this->handle_graph_char(c) )
		return true;
	else if ( this->handle_control_char(c) )
		return true;

	return false;
}

pid_t Terminal::fork( const char * command ) {

	struct winsize ws;

	ws.ws_row    = this->height;
	ws.ws_col    = this->width;
	ws.ws_xpixel = 0;
	ws.ws_ypixel = 0;	

	pid_t childpid = forkpty(&this->pty, NULL, NULL, &ws);
	if (childpid < 0) return -1;

	if (childpid == 0) {
		setenv("TERM", "linux", 1);
		execl("/bin/sh", "/bin/sh", "-c", command, NULL);
		std::cerr << "Oh, crap. Failed to fork." << std::endl;
		exit(127);
	}
	/* if we got here we are the parent process */
	this->childpid = childpid;
	return childpid;
}

void Terminal::poke() {
	fd_set ifs;
	struct timeval tvzero;
	char buf[512];
	int bytesread;
	int n = 5; // XXX: Fix?

	if (this->pty < 0)
		return;
	while (n--) {
		FD_ZERO(&ifs);
		FD_SET(this->pty, &ifs);

		tvzero.tv_sec  = 0;
		tvzero.tv_usec = 0;

		if (select(this->pty + 1, &ifs, NULL, NULL, &tvzero) <= 0)
			return;
		
		bytesread = read(this->pty, buf, 512);
		if (bytesread <= 0)
			return;

		for ( int i = 0; i < bytesread; ++i )
			this->insert( buf[i] );
	}
}


void Terminal::insert( char c ) {

	if ( this->handle_special_char(c) )
		return;

	int ix = this->cX;
	int iy = this->cY;
	/*
	 * XXX: Why was the math using this->cX failing?
	 * for some reason we have to bring it into the local
	 * scope...
	 */
	int offset = (( this->width * iy ) + ix );

	this->chars[offset].ch = c;
	this->advance_curs();
}

void Terminal::type( char c ) {
	write(this->pty, &c, 1);
}

void Terminal::advance_curs() {
	this->cX++;

	if ( this->width <= this->cX ) {
		this->cX = 0;
		this->cY++;
	}

	if ( this->height <= this->cY ) {
		this->cY = (this->height - 1);
		this->scroll_up();
	}
}

int Terminal::get_width() {
	return this->width;
}

int Terminal::get_height() {
	return this->height;
}
