#include "CodeUnit.h"
#include "gdi32f.h"
#include "user32f.h"

void CodeUnit__0(CodeUnit *this, Code *code)
{
	Component__0((Component *) this);
	((Component *) this)->paint = (void (*)(Component *, void *, void *)) CodeUnit_paint;
	((Component *) this)->position = (SIZE (*)(Component *, void *, SIZE *)) CodeUnit_position;
	((Component *) this)->size = (SIZE (*)(Component *, void *)) CodeUnit_size;
	this->code = code;
	this->textFont = CreateFontA(20, 0, 0, 0, 300, 0, 0, 0, 1, 0, 0, 5, 0, "Consolas");
	this->codeFont = CreateFontA(27, 0, 0, 0, 300, 0, 0, 0, 1, 0, 0, 5, 0, "Consolas");
	this->x = 30;
	this->y = 92;
	this->width = 898;
	this->height = 326;
}
void CodeUnit_paint(CodeUnit *this, void *window, void *context)
{
	void *range = CreateRoundRectRgn(0, 0, (int) this->width + 1, (int) this->height + 1, 20, 20);
	void *brush = CreateSolidBrush(0x00000000);
	FrameRgn(context, range, brush, 1, 1);
	DeleteObject(brush);

	if (this->code->mouse && this->code->mouse->unit)
	{
		this->code->mouse->unit(this, window, context);
	}
	else
	{
		CodeUnit_DASM(this, window, context);
	}
}
SIZE CodeUnit_position(CodeUnit *this, void *context, SIZE *client)
{
	SIZE position = {(long) this->x, (long) this->y};
	return position;
}
SIZE CodeUnit_size(CodeUnit *this, void *context)
{
	SIZE size = {(long) this->width, (long) this->height};
	return size;
}

int get_reg(struct instruction *inst, BYTE reg, char *buf, BYTE size)
{
	if (inst->set_field & REX)
	{
		reg |= inst->rex.bits.rex_r << 3;
	}
	char namebuf[4];
	char *name = namebuf;
	int len = 3;
	if (reg == 8 || reg == 9)
		len = 2;
	memcpy(name, registers[reg], len);
	switch (size)
	{
		case 0:
		case 1:
		{
			if (reg > 7)
				name[len++] = (size == 0) ? 'B' : 'W';
			if (reg < 8)
			{
				if (size == 0)
					name[len - 1] = 'L';
				name++;
				len--;
			}
			break;
		}
		case 2:
		{
			if (reg > 7)
				name[len++] = 'D';
			else
				name[0] = 'E';
			break;
		}
	}

	memcpy(buf, name, len);
	return len;
}
int decode_reg(struct instruction *inst, char *buf, BYTE size)
{
	BYTE reg = inst->modrm.bits.reg;
	return get_reg(inst, reg, buf, size);
}
/*
* type:
* 0  MOD R/M Memory
*/
int decode_mem(struct instruction *inst, char *buf, BYTE addressSize, BYTE operandSize, BYTE type)
{
	char membuf[32];
	char *mem = membuf;
	int len = 0;
	switch (operandSize)
	{
		case 0:
		{
			strcpy(mem, "BYTE PTR");
			len += 8;
			break;
		}
		case 1:
		case 2:
		case 3:
		{
			if (operandSize > 1)
			{
				mem[len++] = "DQ"[operandSize - 2];
			}
			strcpy(mem + len, "WORD PTR");
			len += 8;
			break;
		}
	}
	mem[len++] = '[';
	switch (inst->modrm.bits.mod)
	{
		case 0:
		{
			if (inst->modrm.bits.rm == 4)
			{
				// SIB with no displacement
			}
			else if (inst->modrm.bits.rm == 5)
			{
				strcpy(mem + len, "RIP+");
				len += 4;
				// Displacement only
				const char hex[16] = "0123456789ABCDEF";
				BYTE *disp = (BYTE *) (&inst->disp);
				disp += 4;
				for (int i = 0; i < 4; i++)
				{
					disp--;
					mem[len++] = hex[((*disp) >> 4) & 0xF];
					mem[len++] = hex[((*disp) >> 0) & 0xF];
				}
			}
			else
			{
				len += get_reg(inst, inst->modrm.bits.rm, mem + len, addressSize);
			}
			break;
		}
	}
	mem[len++] = ']';
	memcpy(buf, mem, len);
	return len;
}
void CodeUnit_DASM(CodeUnit *this, void *window, void *context)
{
	struct instruction *inst = &this->code->inst;
	BYTE opcb = inst->op[inst->op_len - 1];
	void *font = SelectObject(context, this->codeFont);

	char buf[32] = {0};
	DWORD idx = 0;
	const opcode *opc = find_opcode(inst);
	if (opc)
	{
		memcpy(buf + idx, opc->name, opc->length);
		idx += opc->length;
		BYTE ot = operand_type[inst->op_len - 1][opcb];
		if (ot)
		{
			buf[idx++] = ' ';
			switch (ot)
			{
				case 2:
				{
					BYTE addressSize = 3;
					if (find_legacy_prefix(inst, 0x67))
						addressSize -= 1;

					BYTE operandSize = (2 * (opcb & 1));
					if (operandSize == 2)
					{
						if (find_legacy_prefix(inst, 0x66))
							operandSize = 1;
						if (inst->set_field & REX && inst->rex.bits.rex_w)
							operandSize = 3;
					}

					if (opcb & 2)
					{
						idx += decode_reg(inst, buf + idx, operandSize);
						buf[idx++] = ',';
						buf[idx++] = ' ';
						idx += decode_mem(inst, buf + idx, addressSize, operandSize, 0);
					}
					else
					{
						idx += decode_mem(inst, buf + idx, addressSize, operandSize, 0);
						buf[idx++] = ',';
						buf[idx++] = ' ';
						idx += decode_reg(inst, buf + idx, operandSize);
					}
					break;
				}
			}
		}
	}
	SIZE textSize = {0, 0};
	GetTextExtentPoint32A(context, buf, (int) idx, &textSize);

	SIZE pos = {(long) (this->width - textSize.cx) / 2, 10};
	TextOutA(context, pos.cx, pos.cy, buf, (int) idx);

	SelectObject(context, font);
}
void CodeUnit_REX(CodeUnit *this, void *window, void *context)
{
	void *font = SelectObject(context, this->codeFont);
	BYTE rexValue = this->code->inst.rex.value;
	char x = '0';
	SIZE charSize = {0, 0};
	GetTextExtentPoint32A(context, &x, 1, &charSize);
	BYTE rexBitBuf[21] = " 0 1 0 0             ";
	BYTE *prfx = rexBitBuf + 0;
	BYTE *rfxw = rexBitBuf + 9;
	BYTE *rfxr = rexBitBuf + 12;
	BYTE *rfxx = rexBitBuf + 15;
	BYTE *rfxb = rexBitBuf + 18;
	rfxw[1] = '0' + ((rexValue >> 3) & 1);
	rfxr[1] = '0' + ((rexValue >> 2) & 1);
	rfxx[1] = '0' + ((rexValue >> 1) & 1);
	rfxb[1] = '0' + ((rexValue >> 0) & 1);

	SIZE textSize = {0, 0};
	GetTextExtentPoint32A(context, (const char *) rexBitBuf, 21, &textSize);
	textSize.cx += 6;
	textSize.cy += 2;
	SIZE textPos = {(long) ((this->width - textSize.cx) / 2), 10};

	SIZE rexPrefixSize = {0, 0};
	GetTextExtentPoint32A(context, (const char *) prfx, 9, &rexPrefixSize);
	RECT border = {textPos.cx, textPos.cy};
	border.right = border.left + 2 + rexPrefixSize.cx;
	border.bottom = border.top + 2 + rexPrefixSize.cy;

	TextOutA(context, textPos.cx + 1, textPos.cy + 1, (const char *) prfx, 9);
	void *brush = CreateSolidBrush(0);
	void *range = CreateRectRgn(border.left, border.top, border.right, border.bottom);
	FrameRgn(context, range, brush, 1, 1);
	DeleteObject(range);
	textPos.cx += rexPrefixSize.cx + 1;

	SIZE wrxbSize;
	GetTextExtentPoint32A(context, (const char *) rfxw, 3, &wrxbSize);
	BYTE *bit = rfxw;
	for (DWORD i = 0; i < 4; i++)
	{
		TextOutA(context, textPos.cx + 1, textPos.cy + 1, (const char *) bit, 3);
		border.left = textPos.cx;
		border.right = border.left + wrxbSize.cx + 2;
		FrameRect(context, &border, brush);
		bit += 3;
		textPos.cx += wrxbSize.cx + 1;
	}

	DeleteObject(brush);
	SelectObject(context, font);
	textPos.cx = (long) ((this->width - textSize.cx) / 2);
	textPos.cy += charSize.cy + 2;

	font = SelectObject(context, this->textFont);
	long textHeight = 0;

	{
		char aREX_PREFIX[] = "REX PREFIX";
		SIZE size = {0, 0};
		GetTextExtentPoint32A(context, aREX_PREFIX, 10, &size);
		textHeight = size.cy;
		SIZE pos = {(rexPrefixSize.cx - size.cx) / 2, 0};
		TextOutA(context, textPos.cx + 1 + pos.cx, textPos.cy, aREX_PREFIX, 10);
		long cx = textPos.cx + 2 + rexPrefixSize.cx;
		char type[4] = "WRXB";
		GetTextExtentPoint32A(context, type, 1, &size);
		for (DWORD i = 0; i < 4; i++)
		{
			TextOutA(context, cx + 1 + ((wrxbSize.cx - size.cx) / 2), textPos.cy, type + i, 1);
			cx += wrxbSize.cx + 1;
		}
	}

	textPos.cx = 10;
	textPos.cy += textHeight;

	{
		if (this->code->inst.rex.bits.rex_w)
		{
			TextOutA(context, textPos.cx, textPos.cy, "W = 1: 64 Bit Operand Size", 26);
		}
		else
		{
			TextOutA(context, textPos.cx, textPos.cy, "W = 0: Operand size determined by CS.D", 38);
		}
	}
	textPos.cy += textHeight;
	BYTE REXR[] = "R = 0: Extension of the MODR/M REG field";
	REXR[4] += this->code->inst.rex.bits.rex_r;
	TextOutA(context, textPos.cx, textPos.cy, (char *) REXR, 40);

	textPos.cy += textHeight;
	BYTE REXX[] = "X = 0: Extension of the SIB INDEX field";
	REXX[4] += this->code->inst.rex.bits.rex_x;
	TextOutA(context, textPos.cx, textPos.cy, (char *) REXX, 39);

	textPos.cy += textHeight;
	BYTE REXB[] = "B = 0: Extension of the MODR/M R/M field, SIB BASE field, or Opcode REG field";
	REXB[4] += this->code->inst.rex.bits.rex_b;
	TextOutA(context, textPos.cx, textPos.cy, (char *) REXB, 77);

	SelectObject(context, font);
}
void CodeUnit_MODRM(CodeUnit *this, void *window, void *context)
{
	struct instruction *inst = &this->code->inst;
	BYTE MODRMVAL = this->code->inst.modrm.value;
	void *font = SelectObject(context, this->codeFont);
	SIZE charSize = {0, 0};
	char space = ' ';
	GetTextExtentPoint32A(context, &space, 1, &charSize);

	SIZE pos = {0, 0};

	BYTE modrmBuf[] = " 0 0  0 0 0  0 0 0 ";
	BYTE *modBitBuf = modrmBuf + 0;
	BYTE *regBitBuf = modrmBuf + 5;
	BYTE *r_mBitBuf = modrmBuf + 12;
	modBitBuf[1] += (MODRMVAL >> 7) & 1;
	modBitBuf[3] += (MODRMVAL >> 6) & 1;
	regBitBuf[1] += (MODRMVAL >> 5) & 1;
	regBitBuf[3] += (MODRMVAL >> 4) & 1;
	regBitBuf[5] += (MODRMVAL >> 3) & 1;
	r_mBitBuf[1] += (MODRMVAL >> 2) & 1;
	r_mBitBuf[3] += (MODRMVAL >> 1) & 1;
	r_mBitBuf[5] += (MODRMVAL >> 0) & 1;

	SIZE modrmBitSize = {0, 0};
	GetTextExtentPoint32A(context, (char *) modrmBuf, 19, &modrmBitSize);
	modrmBitSize.cx += 4;
	modrmBitSize.cy += 2;

	pos.cx = (long) (this->width - modrmBitSize.cx) / 2;
	pos.cy = 10;
	SIZE modBitSize = {0, 0};
	SIZE regBitSize = {0, 0};
	SIZE r_mBitSize = {0, 0};
	GetTextExtentPoint32A(context, (char *) modBitBuf, 5, &modBitSize);
	GetTextExtentPoint32A(context, (char *) regBitBuf, 7, &regBitSize);
	GetTextExtentPoint32A(context, (char *) r_mBitBuf, 7, &r_mBitSize);
	TextOutA(context, pos.cx + 1, pos.cy + 1, (char *) modBitBuf, 5);

	void *brush = CreateSolidBrush(0);

	TextOutA(context, pos.cx + 1, pos.cy + 1, (char *) modBitBuf, 5);
	TextOutA(context, pos.cx + modBitSize.cx + 2, pos.cy + 1, (char *) regBitBuf, 7);
	TextOutA(context, pos.cx + modBitSize.cx + regBitSize.cx + 3, pos.cy + 1, (char *) r_mBitBuf, 7);

	RECT border = {pos.cx, pos.cy, pos.cx + modBitSize.cx + 2, pos.cy + modBitSize.cy + 2};
	FrameRect(context, &border, brush);

	border.left += 1 + modBitSize.cx;
	border.right = border.left + regBitSize.cx + 2;
	FrameRect(context, &border, brush);

	border.left += regBitSize.cx + 1;
	border.right = border.left + r_mBitSize.cx + 2;
	FrameRect(context, &border, brush);

	DeleteObject(brush);

	SelectObject(context, this->textFont);

	pos.cy += charSize.cy + 2;

	{
		SIZE size = {0, 0};
		const char modbuf[] = "MOD";
		const char regbuf[] = "REG";
		const char r_mbuf[] = "R/M";
		GetTextExtentPoint32A(context, modbuf, 3, &size);
		TextOutA(context, pos.cx + 1 + ((modBitSize.cx - size.cx) / 2), pos.cy, modbuf, 3);
		TextOutA(context, pos.cx + 1 + modBitSize.cx + 1 + ((regBitSize.cx - size.cx) / 2), pos.cy, regbuf, 3);
		TextOutA(context, pos.cx + 1 + modBitSize.cx + 1 + regBitSize.cx + 1 + ((regBitSize.cx - size.cx) / 2), pos.cy, r_mbuf, 3);
		pos.cx = 10;
		pos.cy += charSize.cy;
	}

	switch (this->code->inst.modrm.bits.mod)
	{
		case 0:
		{
			switch (this->code->inst.modrm.bits.rm)
			{
				case 4:
				{
					TextOutA(context, pos.cx, pos.cy, "MOD = 00: SIB with no displacement (R/M = 100)", 46);
					break;
				}
				case 5:
				{
					TextOutA(context, pos.cx, pos.cy, "MOD = 00: Displacement only addressing mode (R/M = 101)", 55);
					break;
				}
				default:
				{
					TextOutA(context, pos.cx, pos.cy, "MOD = 00: Register indirect addressing mode", 43);
				}
			}
			break;
		}
		case 1:
		{
			TextOutA(context, pos.cx, pos.cy, "MOD = 01: 1 Byte signed displacement follows addressing mode byte(s)", 68);
			break;
		}
		case 2:
		{
			TextOutA(context, pos.cx, pos.cy, "MOD = 10: 4 Byte signed displacement follows addressing mode byte(s)", 68);
			break;
		}
		case 3:
		{
			TextOutA(context, pos.cx, pos.cy, "MOD = 11: Register addressing mode", 34);
			break;
		}
	}
	pos.cy += charSize.cy;

	{
		const char buf[] = "REG = 000: ";
		SIZE prefixSize = {0, 0};
		GetTextExtentPoint32A(context, buf, 11, &prefixSize);
		TextOutA(context, pos.cx, pos.cy, buf, 11);
		const opcode *opc = find_opcode_extension(inst);
		if (opc)
		{
			char opext_desc[] = "Opcode extension ";
			SIZE extSize = {0, 0};
			GetTextExtentPoint32A(context, opext_desc, 17, &extSize);
			TextOutA(context, pos.cx + prefixSize.cx, pos.cy, opext_desc, 17);
			TextOutA(context, pos.cx + prefixSize.cx + extSize.cx, pos.cy, opc->name, (int) opc->length);
		}
		else
		{
			TextOutA(context, pos.cx + prefixSize.cx, pos.cy, registers[inst->modrm.bits.reg], 3);
		}
		pos.cy += charSize.cy;
	}

	{
		BYTE buf[] = "R/M = 000: ";
		buf[6] += (MODRMVAL >> 2) & 1;
		buf[7] += (MODRMVAL >> 1) & 1;
		buf[8] += (MODRMVAL >> 0) & 1;
		SIZE prefixSize = {0, 0};
		GetTextExtentPoint32A(context, (char *) buf, 11, &prefixSize);
		TextOutA(context, pos.cx, pos.cy, (char *) buf, 11);
		BYTE rm = this->code->inst.modrm.bits.rm;
		if (this->code->inst.modrm.bits.mod == 0 && (rm == 4 || rm == 5))
		{
			TextOutA(context, pos.cx + prefixSize.cx, pos.cy, "MOD combine", 11);
		}
		else
		{
			TextOutA(context, pos.cx + prefixSize.cx, pos.cy, registers[rm], 3);
		}
	}

	SelectObject(context, font);
}
