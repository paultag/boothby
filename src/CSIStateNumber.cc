#include "CSIStateMachine.hh"
#include "CSIStateNumber.hh"

#include "boothby.hh"

#include <stdlib.h>
#include <vector>
#include <string>
#include <string.h>

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

	if ( c > 'A' && c < 'z' ) {
		/* OK, so the char is 'twixt A and z, which means that
		 * it's alpha. This means that we're done. Let's parse this
		 * away.
		 * 
		 * A = 65
		 * 
		 */
		
		/* Let's note the ending char */
		csi_state_number_cmd   = c;
		csi_machine_next_state = csi_state_entry;
	} else if ( c > '0' && c < '9' ) {
		/* We have an ascii version of a number.
		 *  0 = 48
		 *  9 = 57
		 */
		csi_state_number_pending = c + csi_state_number_pending;
	} else if ( c == ';' ) {
		/* "Commit" the pending char */
		int n = atoi(csi_state_number_pending.c_str());
		csi_state_number_queue.push_back(n); /* XXX: Backwards? */
	} else {
		csi_machine_next_state = csi_state_invalid;
	}

}

void CSIStateNumber::enter_state() {
	/* Nullop */
}

void CSIStateNumber::leave_state() {
	/* Nullop */
}

CSIStateNumber * csi_state_number = new CSIStateNumber();
