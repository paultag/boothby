#include "Logging.hh"

#include <iostream>
#include <fstream>
#include <sstream>

Logging::Logging(std::string s) {
	this->fd = new std::ofstream;
	this->fd->open(s.c_str());
}

void Logging::log(std::string s) {
	*this->fd << "[dbg]: " << s << std::endl;
}
