#include <malloc.h>
#include <iostream>

#include "Terminal.hh"

Terminal::Terminal(int width, int height) {
	this->width  = width;
	this->height = height;
	this->cX     = 0;
	this->cY     = 0;
	this->cMode  = 0x70; // 112

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
		this->chars[i].attr = 112;
	}
}

void Terminal::scrollUp() {
	/*for ( int iy = this->height; iy >= 1; --iy ) {
		for ( int ix = this->width; ix >= 1; --ix ) {
			int dis = ( this->width * iy );
			int las = ( this->width * ( iy - 1 ));
			this->chars[las].ch   = this->chars[dis].ch;
			this->chars[las].attr = this->chars[dis].attr;
			this->chars[dis].ch = ' ';
		}
	}*/
}

void Terminal::render( WINDOW * win ) {
	for ( int iy = 0; iy < this->height; ++iy ) {
		for ( int ix = 0; ix  < this->width; ++ix ) {
			int offset = (( this->width * iy ) + ix );
			mvwaddch(win, iy, ix, this->chars[offset].ch);
		}
	}
}

void Terminal::insert( char c ) {
	int offset = ( this->cY * this->height ) + this->cX;
	this->chars[offset].ch = c;
	this->cX++;
	
	if ( this->cX >= this->width ) {
		this->cX = 0;
		this->cY++;
	}
	
	if ( this->cY >= this->height ) {
		// scroll
		this->cY = this->height;
	}
}
