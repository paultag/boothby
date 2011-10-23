#include "CSIStateMachine.hh"

State * csi_machine_next_state;
State * csi_machine_curr_state;

void csi_machine_turnkey() {
	csi_machine_curr_state = csi_machine_next_state;
}

void csi_machine_process( char c ) {
	csi_machine_curr_state->process(c);

	if ( csi_machine_curr_state != csi_machine_next_state ) {
		/* a machine has requested a state transition.
		   We can hanlde this the right (tm) way. */

		csi_machine_curr_state->leave_state();
		csi_machine_curr_state = csi_machine_next_state;
		csi_machine_curr_state->enter_state();
	}
}
