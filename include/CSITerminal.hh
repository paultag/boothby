#ifndef _CSITERMINAL_HH_
#define _CSITERMINAL_HH_ foo

#include "CSIStateMachine.hh"
#include "Terminal.hh"

class CSITerminal : public Terminal {
	protected:
		bool handle_special_char( unsigned char c );
		bool handle_escape_char(  unsigned char c );
		bool handle_graph_char(   unsigned char c );
		bool handle_control_char( unsigned char c );

	public:
		CSITerminal();
		CSITerminal(int width, int height);		
		virtual void insert( unsigned char c );
		void apply_csi_sequence( CSICommandPair * pair );
};

#endif
