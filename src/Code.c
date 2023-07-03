#include "Code.h"
#include "gdi32f.h"

void *__cdecl memcpy(void *, const void *, QWORD);
#pragma intrinsic(memcpy)

void Code__0(Code *this)
{
	Component__0(&this->super);
	this->super._1 = (void (*)(Component *)) Code__1;
	this->super.paint = (void (*)(Component *, void *, void *)) Code_paint;
	this->super.position = (SIZE (*)(Component *, void *, SIZE *)) Code_position;
	// this->super.click = (void (*)(Component *, void *)) Code_click;
	this->super.size = (SIZE (*)(Component *, void *)) Code_size;
	// this->super.size = Code_size;
	this->font = CreateFontA(30, 0, 0, 0, 300, 0, 0, 0, 1, 0, 0, 5, 0, "Consolas");
	this->count = 0;
	for (WORD i = 0; i < 7; i++)
	{
		CodeType__0(this->children + i);
	}
}
void Code__1(Code *this)
{
	DeleteObject(this->font);
	this->font = 0;
	Component__1(&this->super);
}
void Code_paint(Code *this, void *window, void *hdc)
{
	Code_update(this);
	void *context = CreateCompatibleDC(hdc);
	void *font = SelectObject(context, this->font);

	SIZE pos = {0, 0};
	for (WORD i = 0; i < this->count; i++)
	{
		SIZE csize = this->children[i].super.size(&this->children[i].super, context);
		void *bitmap = SelectObject(context, CreateCompatibleBitmap(hdc, csize.cx, csize.cy));
		this->children[i].super.paint(&this->children[i].super, window, context);
		BitBlt(hdc, pos.cx, pos.cy, csize.cx, csize.cy, context, 0, 0, 0x00CC0020);
		pos.cx += csize.cx;
		pos.cx += (long) this->space;
		bitmap = SelectObject(context, bitmap);
		DeleteObject(bitmap);
	}

	SelectObject(context, font);
	DeleteDC(context);
}
SIZE Code_position(Code *this, void *context, SIZE *rect)
{
	SIZE pos = {0, 0};
	SIZE size = this->super.size((Component *) this, context);
	pos.cx = (rect->cx - size.cx) / 2;
	pos.cy = (rect->cy - size.cy - 30);
	return pos;
}
void Code_click(Code *this, void *window)
{
}
SIZE Code_size(Code *this, void *context)
{
	Code_update(this);
	void *font = SelectObject(context, this->font);
	SIZE size = {0, 0};

	char space = ' ';
	GetTextExtentPoint32A(context, &space, 1, &size);
	this->space = size.cx;
	size.cx = 0;

	for (WORD i = 0; i < this->count; i++)
	{
		if (i)
			size.cx += (long) this->space;
		SIZE csize = this->children[i].super.size((Component *) &this->children[i], context);
		size.cx += csize.cx;
		size.cy = size.cy < csize.cy ? csize.cy : size.cy;
	}

	SelectObject(context, font);
	return size;
}
void Code_update(Code *this)
{
	this->count = 0;

	if (this->inst.prefix_cnt)
	{
		memcpy(this->children[this->count].code, this->inst.prefixes, this->inst.prefix_cnt);
		this->children[this->count].length = this->inst.prefix_cnt;
		this->children[this->count].color = 0xFFFF55;
		this->count++;
	}

	if (this->inst.set_field & REX)
	{
		this->children[this->count].code[0] = this->inst.rex.value;
		this->children[this->count].length = 1;
		this->children[this->count].color = 0xFF5555;
		this->count++;
	}

	if (this->inst.set_field & VEX)
	{
		memcpy(this->children[this->count].code, this->inst.vex, this->inst.vex_cnt);
		this->children[this->count].length = this->inst.vex_cnt;
		this->children[this->count].color = 0xFFFF55;
		this->count++;
	}

	memcpy(this->children[this->count].code, this->inst.op, this->inst.op_len);
	this->children[this->count].length = this->inst.op_len;
	this->children[this->count].color = 0x55FF55;
	this->count++;

	if (this->inst.set_field & MODRM)
	{
		this->children[this->count].code[0] = this->inst.modrm.value;
		this->children[this->count].length = 1;
		this->children[this->count].color = 0x00AAFF;
		this->count++;
	}

	if (this->inst.set_field & SIB)
	{
		this->children[this->count].code[0] = this->inst.sib.value;
		this->children[this->count].length = 1;
		this->children[this->count].color = 0x55FFFF;
		this->count++;
	}

	if (this->inst.set_field & DISP)
	{
		memcpy(this->children[this->count].code, &this->inst.disp, this->inst.disp_len);
		this->children[this->count].length = this->inst.disp_len;
		this->children[this->count].color = 0xFF55FF;
		this->count++;
	}

	if (this->inst.set_field & IMM)
	{
		memcpy(this->children[this->count].code, &this->inst.imm, this->inst.imm_len);
		this->children[this->count].length = this->inst.imm_len;
		this->children[this->count].color = 0x5555FF;
		this->count++;
	}
}