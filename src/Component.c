#include "component.h"
#include "CodeViewWindow.h"
#include "user32f.h"
#include "gdi32f.h"

void Component__0(Component *this)
{
	this->_1 = Component__1;
	this->paint = Component_paint;
	this->move = Component_move;
	this->keyboard = Component_keyboard;
	this->position = Component_position;
	this->click = Component_click;
	this->size = 0;
	this->window = 0;
	this->repaint = 1;
	this->previous = 0;
}
void Component__1(Component *this)
{
	if (this->previous)
	{
		DeleteObject(this->previous);
		this->previous = 0;
	}
}
void Component_paint(Component *this, void *window, void *context)
{
}
void Component_move(Component *this, void *a, void *b, SIZE *c, long d, long e)
{
}
void Component_keyboard(Component *this, void *window, QWORD wp, QWORD lp)
{
}
SIZE Component_position(Component *this, void *context, SIZE *client)
{
	SIZE size = {0, 0};
	return size;
}
void Component_click(Component *this, void *window)
{
	CodeViewWindow *cvw = (CodeViewWindow *) GetWindowLongPtrA(window, 0);
	if (cvw->focus != this)
	{
		this->repaint = 1;
		if (cvw->focus)
		{
			cvw->focus->repaint = 1;
		}
		cvw->focus = this;
		InvalidateRect(window, 0, 0);
	}
}