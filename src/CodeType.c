#include "CodeType.h"
#include "gdi32f.h"

void CodeType__0(CodeType *this)
{
	Component__0(&this->super);
	this->length = 0;
	this->super.paint = (void (*)(Component *, void *, void *)) CodeType_paint;
	// this->super.click = (void (*)(Component *, void *, void *)) CodeType_click;
	this->super.size = (SIZE (*)(Component *, void *)) CodeType_size;
	this->unit = 0;
}
void CodeType_paint(CodeType *this, void *window, void *hdc)
{
	DWORD color = SetBkColor(hdc, this->color);
	const char hex[16] = "0123456789ABCDEF";
	WORD len = this->length * 3 + 1;
	char buf[26] = "                         ";
	char *ch = buf + 1;
	for (BYTE i = 0; i < this->length; i++)
	{
		ch[0] = hex[(this->code[i] >> 4) & 0xF];
		ch[1] = hex[(this->code[i] >> 0) & 0xF];
		ch += 3;
	}
	// RECT pos = {0, 0, 1920, 1080};
	// DrawTextExA(hdc, buf, len, &pos, 0, 0);
	TextOutA(hdc, 0, 0, buf, len);
	SetBkColor(hdc, color);
}
SIZE CodeType_size(CodeType *this, void *context)
{
	const char hex[16] = "0123456789ABCDEF";
	SIZE size;
	WORD len = this->length * 3 + 1;
	char buf[26] = "                         ";
	char *ch = buf + 1;
	for (BYTE i = 0; i < this->length; i++)
	{
		ch[0] = hex[(this->code[i] >> 4) & 0xF];
		ch[1] = hex[(this->code[i] >> 0) & 0xF];
		ch += 3;
	}
	GetTextExtentPoint32A(context, buf, len, &size);
	return size;
}