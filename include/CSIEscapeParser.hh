#ifndef _CSIESCAPEPARSER_HH_
#define _CSIESCAPEPARSER_HH_ foo

#include "CSIStateMachine.hh"
#include "CSIStateEntry.hh"

#include "boothby.hh"

#include <string>

CSICommandPair * csi_escape_parse( String s );

#endif
