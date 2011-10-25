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

#include <string.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>

CSITerminal::CSITerminal() {
	this->_init_Terminal(80, 25);
	/*                   ^^^^^^ good guess :) */
}
CSITerminal::CSITerminal( int width, int height ) {
	this->_init_Terminal(width, height);
}

void CSITerminal::apply_csi_movement_vector( CSICommandPair * pair ) {
	/*
	 * A - Up
	 * B - Down
	 * C - Forward
	 * D - Backward
	 * 
	 * E - Next line
	 * F - Prev line
	 * G - Move to a given Horz
	 * H - Move to a given X/Y
	 */
	char cmd = pair->first;
	int count = 0;
	 switch ( cmd ) {
		 case 'A': /* mv c up */
			this->cY = this->cY <= 0 ?
				this->cY : this->cY - 1;
			break;
		case 'B': /* mv c down */
			this->cY = this->cY >= this->height ?
				this->cY : this->cY + 1; 
			break;
		case 'C': /* mv c fwd */
			this->cX = this->cX >= this->width ?
				this->cX : this->cX + 1;
			break;
		case 'D': /* mv c back */
			this->cX = this->cX <= 0 ?
				this->cX : this->cX - 1;
			break;
		/* OK. Enough with the easy ones */
		case 'E': /* next N line(s) */
			count = pair->second->at(0);
			this->cY = ( this->cY + count ) >= this->width ?
				this->width : this->cY + count;
			break;
		case 'F': /* move up N lines */
			count = pair->second->at(0);
			this->cY = ( this->cY - count ) <= 0 ?
				0 : this->cY - count;
			break;
		case 'G': /* move to a col */
			count = pair->second->at(0);
			this->cX = count >= this->width ?
				this->width : count;
			break;
		case 'H': /* move to an X/Y */
			int x = pair->second->at(0);
			int y;

			try { // XXX: Revise this
				y = pair->second->at(1);
			} catch ( std::out_of_range & oor ) {
				y = 0;
			}

			x = x >= this->width  ? this->width  : x;
			x = x <= 0            ? 0            : x;
			y = y >= this->height ? this->height : y;
			y = y <= 0            ? 0            : y;

			this->cX = x;
			this->cY = y;

			break;
	 }
}

void CSITerminal::apply_csi_erase_vector( CSICommandPair * pair ) {
	char cmd = pair->first;
	int offset = 0;
	
	switch ( cmd ) {
		case 'J': /* Erase Data */
			try {
				offset = pair->second->at(0);
			} catch ( std::out_of_range & oor ) {
				/* Nada */
			}
			if ( offset == 0 ) {
				this->erase_to_from( this->cX, this->cY,
					this->width, this->height );
			} else if ( offset == 1 ) {
				this->erase_to_from( 0, 0, this->cX, this->cY );
			} else if ( offset == 2 ) {
				this->erase_to_from( 0, 0, this->width, this->height );
			}
			break;
		case 'K': /* Erase in line */
			try {
				offset = pair->second->at(0);
			} catch ( std::out_of_range & oor ) {
				/* Nada */
			}
			/* Erases part of the line. If n is zero (or missing),
			 * clear from cursor to the end of the line. If n is one,
			 * clear from cursor to beginning of the line. If n is two,
			 * clear entire line. Cursor position does not change. */
			 
			 if ( offset == 0 ) {
				 this->erase_to_from( this->cX, this->cY,
					this->width, this->cY );
			 } else if ( offset == 1 ) {
				 this->erase_to_from( 0, this->cY,
					this->cX, this->cY );
			 } else if ( offset == 2 ) {
				 this->erase_to_from( 0, this->cY,
					this->width, this->cY );
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
			case 'E': /* CNL */
			case 'F': /* CPL */
			case 'G': /* CHA */
			case 'H': /* CUP */
				this->apply_csi_movement_vector(pair);
				break;
			case 'J': /* ED */
				this->apply_csi_erase_vector(pair);
				break;
			default:
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
bool CSITerminal::handle_control_char( unsigned char c ) { /* XXX: Move this to Terminal */
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
