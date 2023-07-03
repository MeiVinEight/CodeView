#ifndef COMPONENT_H
#define COMPONENT_H

#include <WinType.h>

#include "user32s.h"

typedef struct Component
{
	void (*_1)(struct Component *);
	void (*paint)(struct Component *, void *, void *);
	void (*move)(struct Component *, void *, void *, SIZE *, long, long);
	void (*keyboard)(struct Component *, void *, QWORD, QWORD);
	SIZE (*position)(struct Component *, void *, SIZE *);
	void (*click)(struct Component *, void *);
	SIZE (*size)(struct Component *, void *);
	void *window;
	int repaint;
	void *previous;
	RECT rect;
} Component;

void Component__0(Component *);
void Component__1(Component *);
void Component_paint(Component *, void *, void *);
void Component_move(Component *, void *, void *, SIZE *, long, long);
void Component_keyboard(Component *, void *, QWORD, QWORD);
SIZE Component_position(Component *, void *, SIZE *);
void Component_click(Component *, void *);

#endif //COMPONENT_H
