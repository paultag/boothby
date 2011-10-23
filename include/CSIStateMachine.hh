#ifndef _CSISTATEMACHINE_HH_
#define _CSISTATEMACHINE_HH_ FOO

#include "State.hh"

#include <vector>
#include <map>

/*
 * 
 * digest:
 * 
 *  std::pair<char, std::vector<int>>
 *            ^^^^  ^^^^^^^^^^^^^^^^
 *            \\\\  \\\\\\\\\\\\\\\\\
 *             \\\\  \\\\\\\\\\\\\\\\\
 *              \\\\  +++++++++++++++++---> Integer commands
 *               \\\\
 *                ++++--------------------> One-char command type
 */
typedef std::pair<char, std::vector<int> * > CSICommandPair;

extern State * csi_machine_next_state;
void csi_machine_process( char c );
void csi_machine_turnkey();

#endif
