#include "CSIStateMachine.hh"

/*#include <iostream>
#include <ncurses.h>*/

State * csi_machine_next_state;
State * csi_machine_curr_state;

void csi_machine_turnkey() {
	csi_machine_curr_state = csi_machine_next_state;
	/*move(0,0);
	printw("Turnkey");
	usleep(200000);
	refresh();*/
}

void csi_machine_process( char c ) {
	csi_machine_curr_state->process(c);

	/* move(0,0);
	printw("Process %d", c);
	usleep(200000);
	refresh(); */

	if ( csi_machine_curr_state != csi_machine_next_state ) {
		/* a machine has requested a state transition.
		   We can hanlde this the right (tm) way. */

	/* move(0,0);
	printw("Migration");
	usleep(200000);
	refresh(); */

		csi_machine_curr_state->leave_state();
		csi_machine_curr_state = csi_machine_next_state;
		csi_machine_curr_state->enter_state();
	}
}
