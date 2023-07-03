#ifndef TEXT_H
#define TEXT_H

#include "component.h"
#include "Code.h"

typedef struct Text
{
	Component super;
	Code *view;
	DWORD width;
	DWORD height;
	void *font;
	int mouse;
	BYTE code[32];
	WORD length;
	WORD inputPos;
} Text;

void Text__0(Text *, Code *);
void Text__1(Text *);
void Text_paint(Text *, void *, void *);
void Text_move(Text *, void *, void *, SIZE *, long, long);
void Text_keyboard(Text *, void *, QWORD, QWORD);
SIZE Text_position(Text *, void *, SIZE *);
void Text_click(Text *, void *);
SIZE Text_size(Text *, void *);
void Text_update(Text *);

#endif //TEXT_H
