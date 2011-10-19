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

void Terminal::insert( char c ) {

	int ix = this->cX;
	int iy = this->cY;
	/*
	 * XXX: Why was the math using this->cX failing?
	 * for some reason we have to bring it into the local
	 * scope...
	 */
	int offset = (( this->width * iy ) + ix );

	this->chars[offset].ch = c;
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
