/*
 * Copyright (C) 2011, Paul Tagliamonte <tag@pault.ag>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "boothby.hh"

#include "CSIEscapeParser.hh"
#include "CSITerminal.hh"

#include <sstream>
#include <string.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>

String itos( int number ) {
	std::ostringstream oss;
	oss << number;
	return oss.str();
}

CSITerminal::CSITerminal() {
	this->_init_Terminal(80, 25);
}
CSITerminal::CSITerminal( int width, int height ) {
	this->_init_Terminal(width, height);
}

void CSITerminal::apply_csi_movement_vector( CSICommandPair * pair ) {

	int newX = this->cX;
	int newY = this->cY;

	int p1 = 1;
	int p2 = 1;

	try {
		p1 = pair->second->at(0);
		p2 = pair->second->at(1);
	} catch ( std::out_of_range & oor ) {}

	switch ( pair->first ) {
		case 'A':
			newY = newY - p1;
			break;
		case 'B':
			newY = newY + p1;
			break;
		case 'C':
			newX = newX + p1;
			break;
		case 'D':
			newX = newX - p1;
			break;
			/* Moves the cursor n (default 1) cells in the given
			 * direction. If the cursor is already at the edge of
			 * the screen, this has no effect. */
		case 'E':
			newX = 0;
			newY = newY + p1;
			break;
			/* Moves cursor to beginning of the line n
			 * (default 1) lines down. */
		case 'd':
		case 'F':
			newX = 0;
			newY = newY - p1;
			break;
			/* Moves cursor to beginning of the line n (default 1)
			 * lines up. */
		case 'G':
			/* Moves the cursor to column n. */
			newX = p1;
			break;
		case 'H':
			/* Moves the cursor to row n, column m.
			 * The values are 1-based, and default to 1
			 * (top left corner) if omitted. A sequence such as
			 * CSI ;5H is a synonym for CSI 1;5H as well as CSI 17;H
			 * is the same as CSI 17H and CSI 17;1H */
			if ( p1 == -1 )
				p1 = 1;
			if ( p2 == -1 )
				p2 = 1;

			newY = p1 - 1;
			newX = p2 - 1;

			break;
		default:
			return;
			break;
	}

	if ( newX > this->width - 1 )
		newX = this->width - 1;
	if ( newX < 0 )
		newX = 0;
	if ( newY > this->height - 1 )
		newY = this->height - 1;
	if ( newY < 0 )
		newY = 0;

	this->cX = newX;
	this->cY = newY;

}

void CSITerminal::apply_csi_decstbm_vector( CSICommandPair * pair ) {
}

void CSITerminal::apply_csi_erase_vector( CSICommandPair * pair ) {
	char cmd = pair->first;
	int offset = 0;
	
	switch ( cmd ) {
		case 'J':
			/* Clears part of the screen.
			 *
			 *  - If n is zero (or missing), clear from cursor to
			 *    end of screen.
			 *
			 *  - If n is one, clear from cursor to beginning of
			 *    the screen.
			 *
			 *  - If n is two, clear entire screen (and moves
			 *    cursor to upper left on MS-DOS ANSI.SYS). */
			try {
				offset = pair->second->at(0);
			} catch ( std::out_of_range & oor ) {}

			if ( offset == -1 )
				offset = 0;

			if ( offset == 0 ) {
				this->erase_to_from( this->cX + 1, this->cY + 1,
					this->width, this->height );
			} else if ( offset == 1 ) {
				this->erase_to_from( 1, 1, this->cX, this->cY );
			} else if ( offset == 2 ) {
				this->erase_to_from( 1, 1, this->width,
				this->height );
			}
			break;
		case 'K':
			try {
				offset = pair->second->at(0);
			} catch ( std::out_of_range & oor ) {}
			/* Erases part of the line. If n is zero (or missing),
			 *
			 *  - clear from cursor to the end of the line.
			 *
			 *  - If n is one, clear from cursor to beginning of the
			 *    line.
			 *
			 *  - If n is two, clear entire line.
			 *
			 * Cursor position does not change. */
			 
			 if ( offset == 0 ) {
				 this->erase_to_from(this->cX + 1, this->cY + 1,
					this->width, this->cY + 1);
			 } else if ( offset == 1 ) {
				 this->erase_to_from(1, this->cY + 1,
					this->cX + 1, this->cY + 1);
			 } else if ( offset == 2 ) {
				 this->erase_to_from(1, this->cY + 1,
					this->cY + 1, this->width);
			 }
			 
			break;
		default:
			/* Humm. Odd. */
			break;
	}
}

void CSITerminal::apply_csi_color_vector( CSICommandPair * pair ) {
	// do things
	std::vector<int> * colors = pair->second;
	
	for ( unsigned int i = 0; i < colors->size(); ++i ) {
		int id = colors->at(i);

		/* OK, some color logic */
		switch ( id ) {
			case 0:
				this->cMode = 0x70;
				break;
			case 1:
			case 2:
			case 4:
				ATTR_MOD_BOLD(this->cMode, 1);
				break;
			case 5:
				ATTR_MOD_BLINK(this->cMode, 1);
				break;
			default:
				/* Unknown */
				break;
		}
	}
}

void CSITerminal::apply_csi_sequence( CSICommandPair * pair ) {
		/* OK. Let's apply this to the current term */
		switch ( pair->first ) {
			case 'm':
				/* Color-changer */
				this->apply_csi_color_vector(pair);
				break;
			case 'A': /* CUU */
			case 'B': /* CUD */
			case 'C': /* CUF */
			case 'D': /* CUB */
			case 'd':
			case 'E': /* CNL */
			case 'F': /* CPL */
			case 'G': /* CHA */
			case 'H': /* CUP */
			case 'f': /* Same as CUP */
				this->apply_csi_movement_vector(pair);
				break;
			case 'J': /* ED */
			case 'K': /* EL */
				this->apply_csi_erase_vector(pair);
				break;
			case 'r':
				this->apply_csi_decstbm_vector(pair);
				break;
			default:
				DEBUG("UH: " << pair->first)
				/* Damn! */
				break;
		}
}

bool CSITerminal::handle_escape_char( unsigned char c ) {
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
		try {
			CSICommandPair * CSIEscapeSequence = 
				csi_escape_parse( this->escape );
			this->apply_csi_sequence( CSIEscapeSequence );
		} catch ( int i ) { // XXX: Fixme
			/* Invalid char :( */
		}
		
		this->special = false;
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
bool CSITerminal::handle_graph_char( unsigned char c ) {
	return false;
}

#define _GET_AT_OFFSET ((this->cY * this->width) + this->cX)
/* XXX: FIXME ^ */

bool CSITerminal::handle_control_char( unsigned char c ) { /* XXX: Move this to Terminal */
	switch ( c ) {
		case '\n': /* newline */
			this->cX = this->width; /* XXX: Fix this hack */
			this->advance_curs();
			return true;
			break;
		case '\b': /* backspace */
			if ( this->cX )
				this->cX--; // XXX: Beginning of line?
			this->chars[_GET_AT_OFFSET].attr = 0x70;
			this->chars[_GET_AT_OFFSET].ch   = ' '; // XXX: Is this right?
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
			/* Rote seems to do this. Let's copy the case. */
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

bool CSITerminal::handle_special_char( unsigned char c ) {
	if ( this->handle_escape_char(c) )
		return true;
	else if ( this->handle_graph_char(c) )
		return true;
	else if ( this->handle_control_char(c) )
		return true;

	return false;
}

void CSITerminal::insert( unsigned char c ) {
	if ( this->handle_special_char(c) )
		return;
	Terminal::insert(c);
}
