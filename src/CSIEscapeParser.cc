#include "CSIEscapeParser.hh"
#include "CSIStateNumber.hh"
#include "CSIStateEntry.hh"

CSICommandPair * csi_escape_parse( String s ) {
	csi_machine_next_state = csi_state_entry;
	csi_machine_turnkey();
	
	for ( unsigned int i = 0; i < s.length(); ++i )
		csi_machine_process(s[i]);

	CSICommandPair * parsed_csi_string = new CSICommandPair(
		csi_state_number_cmd,
		&csi_state_number_queue
	);

	return parsed_csi_string;
}
