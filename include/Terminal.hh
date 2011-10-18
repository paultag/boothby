#ifndef _TERMINAL_HH_
#define _TERMINAL_HH_ foo

class TerminalChar {
	protected:
		unsigned char attr;
		unsigned char cha;

	public:
		TerminalChar();
		void setAttr( unsigned char c );
		unsigned char getAttr();
		
		void setChar( unsigned char c );
		unsigned char getChar();
};

class Terminal {
	protected:
		int width;
		int height;

		int cX;
		int cY;
		
		unsigned char cMode;

		TerminalChar ** screen;

	public:
		Terminal(int width, int height);
};

#endif
