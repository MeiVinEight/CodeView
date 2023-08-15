#ifndef CODEUNIT_H
#define CODEUNIT_H

#include "component.h"
#include "Code.h"

typedef struct CodeUnit
{
	Component super;
	Code *code;
	void *textFont;
	void *codeFont;
	DWORD x;
	DWORD y;
	DWORD width;
	DWORD height;
} CodeUnit;

void CodeUnit__0(CodeUnit *, Code *);
void CodeUnit_paint(CodeUnit *, void *, void *);
SIZE CodeUnit_position(CodeUnit *, void *, SIZE *);
SIZE CodeUnit_size(CodeUnit *, void *);

void CodeUnit_DASM(CodeUnit *, void *, void *);
void CodeUnit_REX(CodeUnit *, void *, void *);
void CodeUnit_MODRM(CodeUnit *, void *, void *);

#endif //CODEUNIT_H
