#ifndef _CSISTRING_HH_
#define _CSISTRING_HH_ FOO

#include <string>
#include "boothby.hh"

class CSIString {
	protected:
		int * strinf;
		
		void parse(String s);
		
	public:
		CSIString(String s);

};

#endif
