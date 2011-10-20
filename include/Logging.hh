#ifndef _LOGGING_HH_
#define _LOGGING_HH_ foo

#include <iostream>
#include <fstream>

class Logging {
	protected:
		std::ofstream * fd;

	public:
		Logging(std::string s);
		void log(std::string msg);
};

#endif
