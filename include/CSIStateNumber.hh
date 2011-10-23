#ifndef CSISTATENUMBER_HH_
#define CSISTATENUMBER_HH_ ja

#include "boothby.hh"
#include "State.hh"

#include <vector>
#include <string>

class CSIStateNumber : public State {
	public:
		virtual void leave_state();
		virtual void enter_state();
		virtual void process( char c );
};

extern CSIStateNumber * csi_state_number;
/* OK, we need a vector for the parsed junk */

extern std::vector<int> csi_state_number_queue;
extern String           csi_state_number_cmd;

#endif
