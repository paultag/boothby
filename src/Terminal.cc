#include <malloc.h>
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
 
	for ( int ix = 0; ix < width; ++ix ) {
		int xOffset = ( ix * width );
		for ( int iy = 0; iy < height; ++iy ) {
			this->chars[xOffset + iy].ch   = ' ';
			this->chars[xOffset + iy].attr = 112;
		}
	}
}

void Terminal::insert( char c ) {
	int offset = ( this->cX * this->width ) + this->cY;
	this->chars[offset].ch = c;
	this->cX++;
	if ( this->cX >= this->width ) {
		this->cX = 0;
		this->cY++;
	}
	if ( this->cY >= this->height ) {
		// scroll_up
		this->cY = this->height;
	}
}
