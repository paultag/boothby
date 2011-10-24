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
#include <vector>
#include <map>

CSITerminal::CSITerminal() {
	this->_init_Terminal(80, 25);
	/*                   ^^^^^^ good guess :) */
}
CSITerminal::CSITerminal( int width, int height ) {
	this->_init_Terminal(width, height);
}

void CSITerminal::apply_csi_color_vector( CSICommandPair * pair ) {
	// do things
	std::vector<int> * colors = pair->second;
	
	for ( unsigned int i = 0; i < colors->size(); ++i ) {
		int id = colors->at(i);

		std::cerr << "Setting attr: " << id << std::endl;

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
			std::cerr << "Error parsing " << this->escape << std::endl; /// XXX: THIS
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
bool CSITerminal::handle_control_char( unsigned char c ) {
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
