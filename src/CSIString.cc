#include "CSIString.hh"

CSIString::CSIString(String s) {
	this->parse(s);
}

void CSIString::parse(String s ) {
	/*
	 * Looks like:
	 * 
	 * [31m
	 * [1;31m
	 * 
	 * or similar.
	 */
	
	if ( s.substr(0,2) == "[?" )
		return;
		/* OK. This is a private-mode CSI string. */
		
	

	for ( unsigned int i = 0; i < s.length(); ++i ) {
		if ( s.substr(i,i+1) == ";" ) {
			/* OK, we've ended this phrase. */
			
		}
	}
}
