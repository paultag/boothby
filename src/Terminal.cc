#include <malloc.h>
#include <iostream>
#include <pty.h>
#include <stdlib.h>

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
	this->escape    = (char *)malloc(sizeof(char) * 8); // XXX: Overflow...
	this->escape[0] = '\0';

	/* OK. This cMode can be a bit of a bear to use. Let's explain it
	 * a bit.
	 * 
	 *     FG     BG
	 *  +------+------+
	 *  | BBBM | FFFN |
	 *    ^ 0th bit
	 *    0000 | 1110 
	 *           16 32 64
	 *    = 112
	 *
	 * 0 -> 3 = Background Color Code ( 0 - 7 )
	 * 4      = Blink
	 * 5 -> 7 = Foreground Color Code ( 0 - 7 )
	 * 8      = Bold
	 */
	this->chars = (TerminalCell*)
		malloc(sizeof(TerminalCell) * (width * height));
 
	for ( int i = 0; i < ( height * width ); ++i ) {
		this->chars[i].ch   = ' ';
		this->chars[i].attr = this->cMode;
	}
}

void Terminal::scrollUp() {
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

bool Terminal::handle_special_char( char c ) {

	switch ( c ) {
		case '\n': /* Newline */
			this->cX = this->width; /* XXX: Fix this hack */
			this->advance_curs();
			return true;
			break;
		case '\b': /* Backspace */
			if ( this->cX )
				this->cX--;
			return true;
			break;
		case '\t': /* Tab */
			while (this->cX % 8)
				this->insert(' ');
			return true;
			break;
		case '\x1B': /* begin escape sequence (aborting previous one if any) */
			this->special = true;
			break;
		case '\x0E': /* enter graphical character mode */
			this->graph = true;
			return true;
			break; 
		case '\x0F': /* exit graphical character mode */
			this->graph = false;
			return true;
			break;
		case '\x9B': /* CSI character. Equivalent to ESC [ */
			return true;
			break;
		case '\x18': case '\x1A': /* these interrupt escape sequences */
			this->special = false;
			return true;
			break;
		case '\a': /* bell */
			return true;
			break;
		default:
			break;

	}

	// some last ditch crap 
	if ( c < 32 )
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

void Terminal::advance_curs() {
	this->cX++;

	if ( this->width <= this->cX ) {
		this->cX = 0;
		this->cY++;
	}

	if ( this->height <= this->cY ) {
		this->cY = (this->height - 1);
		this->scrollUp();
	}
}

int Terminal::getWidth() {
	return this->width;
}

int Terminal::getHeight() {
	return this->height;
}
