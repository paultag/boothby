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

void CSITerminal::apply_csi_sequence( CSICommandPair * pair ) {
	
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
			/*
			OK, we have a valid thinger
			char cmd = CSIEscapeSequence->first;
			move(0,0);
			printw("Cmd: %s", &cmd);
			refresh();
			usleep(2000000); */
		} catch ( int i ) { // XXX: Fixme
			// Invalid char :(
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
