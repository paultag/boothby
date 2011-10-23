#ifndef CSISTATEENTRY_HH_
#define CSISTATEENTRY_HH_ ja

#include "State.hh"

class CSIStateEntry : public State {
	public:
		virtual void leave_state();
		virtual void enter_state();
		virtual void process( char c );
};

extern CSIStateEntry * csi_state_entry;

#endif
