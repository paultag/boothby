#ifndef _RENDERABLETERMINAL_HH_
#define _RENDERABLETERMINAL_HH_ foo

#include "Terminal.hh"
#include "Renderable.hh"
#include "Pane.hh"

class RenderableTerminal : public Renderable {
	protected:
		Terminal * terminal;
		Pane     * pane;

		int width;
		int height;

	public:
		RenderableTerminal( Terminal * t );
		~RenderableTerminal();
		virtual void render();
		void insert(unsigned char c);
		
		void move_to(unsigned int x, unsigned int y );
		
};

#endif
