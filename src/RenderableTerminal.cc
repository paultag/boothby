#include "RenderableTerminal.hh"
#include "Terminal.hh"
#include "Pane.hh"

RenderableTerminal::RenderableTerminal( Terminal * t ) {
	this->terminal = t;
	int width  = t->getWidth();
	int height = t->getHeight();

	this->width  = width;
	this->height = height;

	Pane * p = new Pane(width, height, 0, 0 );
	/*                                 ^^^^^
	                            Default position. Fixme */
	this->pane = p;
}

void RenderableTerminal::render() {
	this->terminal->render(this->pane->getWindow());
}
