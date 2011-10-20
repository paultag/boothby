#include "boothby.hh"

#include "CSITerminal.hh"

#include <string.h>

CSITerminal::CSITerminal() {
	this->_init_Terminal(80, 25);
}

CSITerminal::CSITerminal( int width, int height ) {
	this->_init_Terminal(width, height);
}

void CSITerminal::csi_atize() {
	
	char * csi = this->escape;
	
	int slen = strlen(csi);
	
	if ( slen == 0 ) {
		this->cMode = 0x70;
		return;
	}
	
	for ( int i = 0; i < slen; ++i ) {
		
		char c = csi[i];
		
		if (c == 0) {
			this->cMode = 0x70;
		} else if (c == 1 || c == 2 || c == 4) {
			ATTR_MOD_BOLD(this->cMode,1);
		} else if (c == 5) {  /* set blink */
			ATTR_MOD_BLINK(this->cMode,1);
		} else if (c == 7 || c == 27) { /* reverse video */
			int fg = ATTR_FG(this->cMode);
			int bg = ATTR_BG(this->cMode);
			ATTR_MOD_FG(this->cMode, bg);
			ATTR_MOD_BG(this->cMode, fg);
		} else if (c == 8) {
			this->cMode = 0x0;    /* invisible */
		} else if (c == 22 || c == 24) { /* bold off */
			ATTR_MOD_BOLD(this->cMode,0);
		} else if (c == 25) { /* blink off */
			ATTR_MOD_BLINK(this->cMode,0);
		} else if (c == 28) { /* invisible off */
			this->cMode = 0x70;
		} else if (c >= 30 && c <= 37) {    /* set fg */
			move(0,0);
			printw("Set the foreground");
			refresh();
			ATTR_MOD_FG(this->cMode, c - 30);
		} else if (c >= 40 && c <= 47) {    /* set bg */
			ATTR_MOD_BG(this->cMode, c - 40);
		} else if (c == 39) {  /* reset foreground to default */
			ATTR_MOD_FG(this->cMode, 7);
		} else if (c == 49) {  /* reset background to default */
			ATTR_MOD_BG(this->cMode, 0);
		}
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
		this->special = false;
		this->csi_atize();
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
	
	/* This was yanked directly from rote */
	
	if ( ! this->graph )
		return false;
	
	char nc;
	switch (c) {
		case 'k': case 'l': case 'm': case 'n':
		case 't': case 'u': case 'v': case 'w':
		case 'j':
			nc = '+';
			break;
		case 'x':
			nc = '|';
			break;
		default:
			nc = '%';
			break;
	}

	this->insert(nc); // come back at it like a mofo.

	return true;
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
