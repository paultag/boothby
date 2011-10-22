#ifndef STATE_H_
#define STATE_H_ ja

class State {
	public:
		virtual void leave_state();
		virtual void enter_state();
		virtual void process( char c );
};

#endif
