#ifndef CODETYPE_H
#define CODETYPE_H

#include "component.h"

typedef struct CodeType
{
	Component super;
	BYTE length;
	BYTE code[15];
	DWORD color;
} CodeType;

void CodeType__0(CodeType *this);
void CodeType_paint(CodeType *, void *, void *);
SIZE CodeType_position(CodeType *, void *, void *, RECT *, SIZE *);
void CodeType_click(CodeType *, void *);
SIZE CodeType_size(CodeType *, void *);

#endif //CODETYPE_H
