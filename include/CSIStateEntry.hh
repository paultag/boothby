#ifndef CSISTATEENTRY_HH_
#define CSISTATEENTRY_HH_ ja

class CSIStateEntry : public State {
	public:
		virtual void leave_state();
		virtual void enter_state();
		virtual void process( char c );
};

#endif
