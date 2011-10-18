#ifndef _TERMINAL_HH_
#define _TERMINAL_HH_ foo

typedef struct _TerminalCell {
	unsigned char attr;
	unsigned char ch;
} TerminalCell;

class Terminal {
	protected:
		int width;
		int height;

		int cX;
		int cY;
		
		unsigned char cMode;
		
		TerminalCell ** cells;
	public:
		Terminal(int width, int height);
};

#endif
