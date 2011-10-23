#include "CSIStateMachine.hh"
#include "CSIStateEntry.hh"

/* OK, state transitions follow */

#include "CSIStateInvalid.hh"
#include "CSIStateNumber.hh"

void CSIStateEntry::process( char c ) {
	/*
	 * 
	 * IF:
	 *  It's a [, good. Move to a CSINumber state
	 * Else:
	 * 	Transition to a CSIInvalid state 
	 */
	if ( c == '[' ) {
		csi_machine_next_state = csi_state_number;
	} else {
		csi_machine_next_state = csi_state_invalid;
	}
}

void CSIStateEntry::enter_state() {
	/* Nullop */
}

void CSIStateEntry::leave_state() {
	/* Nullop */
}

CSIStateEntry * csi_state_entry = new CSIStateEntry();
