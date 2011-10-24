/*
 * Copyright (C) 2011, Paul Tagliamonte <tag@pault.ag>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "CSITerminal.hh"
#include "RenderableTerminal.hh"
#include "CSIEscapeParser.hh"

#include <iostream>

#include "boothby.hh"

int main ( int argc, char ** argv ) {
	init_screen();
	try {

		std::vector<Terminal           *> TerminalStack;
		std::vector<RenderableTerminal *> RTerminalStack;

		Terminal * tCur;


		for ( unsigned int i = 0; i < 1; ++i ) {
			Terminal           *  t  = new CSITerminal(80, 25);
			RenderableTerminal *  rt = new RenderableTerminal(t);

			TerminalStack.push_back(t);
			RTerminalStack.push_back(rt);

			rt->move_to(i*3, i*5);
			t->fork("bash");
			tCur = t;
		}

		update_screen();

		while ( true ) {

			for ( unsigned int i = 0; i < TerminalStack.size(); ++i ) {
				TerminalStack.at(i)->poke();
			}

			for ( unsigned int i = 0; i < RTerminalStack.size(); ++i ) {
				RTerminalStack.at(i)->render();
			}

			update_screen();
			timeout(0);
			char ch = getch();
			if ( ch != ERR )
				tCur->type(ch);
			usleep(2000);
		}
		uninit_screen();
	} catch ( int i ) {
		uninit_screen();
		std::cout << "ERR: " << i << std::endl;
	}
}
