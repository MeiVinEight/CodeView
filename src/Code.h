#ifndef CODE_H
#define CODE_H

#include "component.h"
#include "CodeType.h"
#include "inst.h"

typedef struct Code
{
	Component super;
	void *font;
	struct instruction inst;
	WORD count;
	CodeType children[8];
	CodeType *mouse;
	Component *unit;
	DWORD space;
} Code;

void Code__0(Code *, Component *);
void Code__1(Code *);
void Code_paint(Code *, void *, void *);
void Code_move(Code *, void *, void *, SIZE *, long, long);
SIZE Code_position(Code *, void *, SIZE *);
void Code_click(Code *, void *);
SIZE Code_size(Code *, void *);
void Code_update(Code *);

#endif //CODE_H
