#ifndef _CSITERMINAL_HH_
#define _CSITERMINAL_HH_ foo

#include "Terminal.hh"

class CSITerminal : public Terminal {
	protected:
		bool handle_special_char( unsigned char c );
		bool handle_escape_char(  unsigned char c );
		bool handle_graph_char(   unsigned char c );
		bool handle_control_char( unsigned char c );
		void csi_atize();

	public:
		CSITerminal();
		CSITerminal(int width, int height);		
};

#endif
