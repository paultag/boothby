#include "RenderableTerminal.hh"
#include "Terminal.hh"
#include "Pane.hh"

RenderableTerminal::RenderableTerminal( Terminal * t ) {
	this->terminal = t;
	int width  = t->get_width();
	int height = t->get_height();
	this->width  = width;
	this->height = height;

	Pane * p = new Pane(0, 0, width, height );
	this->pane = p;
}

RenderableTerminal::~RenderableTerminal() {
	delete this->pane;
}

void RenderableTerminal::render() {
	this->terminal->render(this->pane->getWindow());
}

void RenderableTerminal::insert( unsigned char c ) {
	this->terminal->insert(c);
}

void RenderableTerminal::move_to(unsigned int x, unsigned int y ) {
	this->pane->move_to(x, y);
}
