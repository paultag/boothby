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

/*#include <iostream>
#include <ncurses.h>*/

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
