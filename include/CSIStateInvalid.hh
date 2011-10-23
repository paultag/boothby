#ifndef CSISTATEINVALID_HH_
#define CSISTATEINVALID_HH_ ja

#include "State.hh"

class CSIStateInvalid : public State {
	public:
		virtual void leave_state();
		virtual void enter_state();
		virtual void process( char c );
};

extern CSIStateInvalid * csi_state_invalid;

#endif
