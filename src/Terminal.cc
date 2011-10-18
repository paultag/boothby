#include <malloc.h>

#include "Terminal.hh"

TerminalChar::TerminalChar() {
	/*
	 * Nothing much to do
	 */
}

void TerminalChar::setChar( unsigned char c ) {
	this->cha = c;
}
	
void TerminalChar::setAttr( unsigned char c ) {
	this->attr = c;
}

unsigned char TerminalChar::getChar() {
	return this->cha;
}

unsigned char TerminalChar::getAttr() {
	return this->attr;
}

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
	 
	 /* OK, crap. Let's set up the screen now. Huge amount of objects
	    are about to be new'd. */
	
	this->screen = (TerminalChar **)
		malloc(sizeof(TerminalChar) * (width * height));
	
	for ( int ix = 0; ix < width; ++ix )
		for ( int iy = 0; iy < height; ++iy )
			this->screen[ix][iy] = new TerminalChar();
}
