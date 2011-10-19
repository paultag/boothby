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
		virtual void render();
		
};

#endif
