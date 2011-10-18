#include <malloc.h>
#include <iostream>
#include "Terminal.hh"

Terminal::Terminal(int width, int height) {
	this->width  = width;
	this->height = height;
	
	this->cX = 0;
	this->cY = 0;

	this->cMode = 0;
	/*
	 * OK. This cMode can be a bit of a bear to use. Let's explain it
	 * a bit.
	 * 
	 * 
	 *     FG     BG
	 *  +------+------+
	 *  | BBBM | FFFN |
	 *    ^ 0th bit
	 * 
	 * 0 -> 3 = Background Color Code ( 0 - 7 )
	 * 4      = Blink
	 * 5 -> 7 = Foreground Color Code ( 0 - 7 )
	 * 8      = Bold
	 * 
	 */
	 
	this->cells = (TerminalCell **)
		malloc(sizeof(TerminalCell) * ( width * height ));
	 
	for ( int ix = 0; ix < width; ++ix ) {
		for ( int iy = 0; iy < height; ++iy ) {
			std::cout << ix << ", " << iy << std::endl;
			cells[ix][iy].ch   = 0;
			cells[ix][iy].attr = 0;
		}
	}
}
