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

#include "CSIStateMachine.hh"
#include "CSIStateNumber.hh"

#include "boothby.hh"

#include <stdlib.h>
#include <vector>
#include <string>
#include <string.h>
#include <ncurses.h>

/* more kruft */

#include "CSIStateInvalid.hh"
#include "CSIStateEntry.hh"

std::vector<int> csi_state_number_queue;
String           csi_state_number_cmd;

String csi_state_number_pending;

void CSIStateNumber::process( char c ) {
	/*
	 * IF:
	 *   EOF:
	 *     Back to CSIStateEntry
	 *   Number:
	 *    Push back on pending number
	 *   Semicol:
	 *    Parse number and push back in array
	 *   a-Z:
	 *    consider this an ender. Back to CSIStateEntry
	 *   Else:
	 *    InvalidState
	 *
	 */

	if ( (c >= 'A' && c <= 'Z') ||
	     (c >= 'a' && c <= 'z')
	) {
		/* OK, so the char is 'twixt A and z, which means that
		 * it's alpha. This means that we're done. Let's parse this
		 * away.
		 * 
		 * A = 65
		 * 
		 */
		
		/* Let's note the ending char */
		csi_state_number_cmd   = c;

		/* "Commit" the pending char */
		int n = atoi(csi_state_number_pending.c_str());

		if ( csi_state_number_pending == "" )
			n = -1;

		csi_state_number_queue.insert(csi_state_number_queue.end(), n);
		csi_state_number_pending = "";
		csi_machine_next_state = csi_state_entry;
	} else if ( c >= '0' && c <= '9' ) {
		/* We have an ascii version of a number.
		 *  0 = 48
		 *  9 = 57
		 */
		csi_state_number_pending =  csi_state_number_pending + c;
	} else if ( c == ';' ) {
		/* "Commit" the pending char */
		int n = atoi(csi_state_number_pending.c_str());
		if ( csi_state_number_pending == "" )
			n = -1;

		csi_state_number_queue.insert(csi_state_number_queue.end(), n);
		csi_state_number_pending = "";
	} else {
		csi_machine_next_state = csi_state_invalid;
	}

}

void CSIStateNumber::enter_state() {
	csi_state_number_cmd = '\0';
	csi_state_number_queue.erase(
		csi_state_number_queue.begin(),
		csi_state_number_queue.end()
	);
}

void CSIStateNumber::leave_state() {
	/* Nullop */
}

CSIStateNumber * csi_state_number = new CSIStateNumber();
