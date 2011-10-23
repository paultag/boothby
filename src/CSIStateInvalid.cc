#include "CSIStateInvalid.hh"

void CSIStateInvalid::process( char c ) {
	/* Throw an exception */
	throw -1;
}

void CSIStateInvalid::enter_state() {
	/* Nullop */
}

void CSIStateInvalid::leave_state() {
	/* Nullop */
}

CSIStateInvalid * csi_state_invalid = new CSIStateInvalid();
