#include "Text.h"
#include "gdi32f.h"
#include "user32f.h"
#include "CodeViewWindow.h"

void Text__0(Text *this, Code *view)
{
	Component__0((Component *) this);
	((Component *) this)->_1 = (void (*)(Component *)) Text__1;
	((Component *) this)->paint = (void (*)(Component *, void *, void *)) Text_paint;
	((Component *) this)->move = (void (*)(Component *, void *, void *, SIZE *, long, long)) Text_move;
	((Component *) this)->keyboard = (void (*)(struct Component *, void *, QWORD, QWORD)) Text_keyboard;
	((Component *) this)->position = (SIZE (*)(Component *, void *, SIZE *)) Text_position;
	((Component *) this)->size = (SIZE (*)(Component *, void *)) Text_size;
	this->view = view;
	this->width = 0x200;
	this->height = 0x20;
	this->font = CreateFontA(20, 0, 0, 0, 300, 0, 0, 0, 1, 0, 0, 5, 0, "Consolas");
	this->mouse = 0;
	for (WORD i = 0; i < 32; this->code[i++] = 0);
	this->length = 0;
	this->inputPos = 0;

	Text_update(this);
	// this->code[0] = 0x4B;
	// this->code[1] = 0x5A;
	// this->length = 2;
}
void Text__1(Text *this)
{
	DeleteObject(this->font);
	this->font = 0;
	Component__1(&this->super);
}
void Text_paint(Text *this, void *window, void *context)
{
	if (((Component *) this)->repaint)
	{
		DWORD colorWhenHover = 0x00F8F8F8;
		CodeViewWindow *cvw = (CodeViewWindow *) GetWindowLongPtrA(window, 0);
		void *font = SelectObject(context, this->font);

		// Text box size
		SIZE size = this->super.size(&this->super, context);

		// create a round rect range
		void *range = CreateRoundRectRgn(0, 0, size.cx + 1, size.cy + 1, 6, 6);

		if (cvw->focus != ((Component *) this) && this->mouse)
		{
			// fill background with gray
			void *brush = SelectObject(context, CreateSolidBrush(colorWhenHover));
			PaintRgn(context, range);
			DeleteObject(SelectObject(context, brush));
		}

		// fill range's border
		void *brush = CreateSolidBrush(0x00EEEEEE);
		FrameRgn(context, range, brush, 1, 1);
		DeleteObject(brush);

		// draw a line at bottom
		if (cvw->focus == ((Component *) this))
		{
			void *pen = SelectObject(context, CreatePen(0, 1, 0x00C06700));
			MoveToEx(context, 0, size.cy - 2, 0);
			LineTo(context, size.cx - 1, size.cy - 2);
			MoveToEx(context, 0, size.cy - 1, 0);
			LineTo(context, size.cx - 1, size.cy - 1);
			DeleteObject(SelectObject(context, pen));
		}
		else
		{
			void *pen = SelectObject(context, CreatePen(0, 1, 0x00888888));
			MoveToEx(context, 0, size.cy - 1, 0);
			LineTo(context, size.cx - 1, size.cy - 1);
			DeleteObject(SelectObject(context, pen));
		}

		// hex text
		char space = ' ';
		char hex[16] = "0123456789ABCDEF";
		char buf[48] = {0};
		WORD len = 0;
		for (WORD i = 0; i < this->length; i++)
		{
			if ((i >> 1) && (1 ^ (i & 1)))
			{
				buf[len++] = ' ';
			}
			buf[len++] = hex[this->code[i] & 0xF];
		}
		SIZE textSize = {0, 0};
		SIZE spaceSize = {0, 0};
		GetTextExtentPoint32A(context, buf, len, &textSize);
		GetTextExtentPoint32A(context, &space, 1, &spaceSize);
		textSize.cy = spaceSize.cy;
		DWORD bkColor = 0x00FFFFFF;
		if (cvw->focus != ((Component *) this) && this->mouse)
		{
			bkColor = colorWhenHover;
		}
		bkColor = SetBkColor(context, bkColor);
		TextOutA(context, 10, (long) (this->height - textSize.cy) / 2, buf, len);
		SetBkColor(context, bkColor);
		if (cvw->focus == (Component *) this)
		{
			RECT vline;
			WORD ip = this->inputPos - (this->inputPos != 0);
			vline.left = 10 + spaceSize.cx * (this->inputPos + (ip >> 1));
			vline.top = (long) (this->height - textSize.cy) / 2;
			vline.right = vline.left + 2;
			vline.bottom = vline.top + textSize.cy;
			void *vlineBrush = CreateSolidBrush(0);
			FillRect(context, &vline, vlineBrush);
			DeleteObject(vlineBrush);
		}

		// delete area out of range
		void *crange = CreateRectRgn(0, 0, size.cx + 1, size.cy + 1);
		CombineRgn(crange, crange, range, 4); // RGN_DIFF
		brush = SelectObject(context, CreateSolidBrush(0x00FFFFFF));
		PaintRgn(context, crange);
		DeleteObject(SelectObject(context, brush));

		DeleteObject(crange);
		DeleteObject(range);

		SelectObject(context, font);
		((Component *) this)->repaint = 0;
	}
}
void Text_move(Text *this, void *window, void *context, SIZE *client, long x, long y)
{
	CodeViewWindow *cvw = (CodeViewWindow *) GetWindowLongPtrA(window, 0);
	SIZE pos = this->super.position((Component *) this, context, client);
	SIZE size = ((Component *) this)->size((Component *) this, context);
	int mouse = range(pos.cx, pos.cy, pos.cx + size.cx, pos.cy + size.cy, cvw->x, cvw->y);
	if (range(pos.cx, pos.cy, pos.cx + size.cx, pos.cy + size.cy, x, y))
	{
		if (!mouse)
		{
			this->mouse = 1;
			SetCursor(LoadCursorA(0, (void *) 32513));
			if (cvw->focus != (Component *) this)
			{
				((Component *) this)->repaint = 1;
				InvalidateRect(window, 0, 0);
			}
		}
	}
	else if (mouse)
	{
		this->mouse = 0;
		SetCursor(LoadCursorA(0, (void *) 32512));
		if (cvw->focus != (Component *) this)
		{
			((Component *) this)->repaint = 1;
			InvalidateRect(window, 0, 0);
		}
	}
	Component_move((Component *) this, window, context, client, x, y);
}
void Text_keyboard(Text *this, void *window, QWORD wp, QWORD lp)
{
	if ((wp >= '0' && wp <= '9') || (wp >= 'A' && wp <= 'F'))
	{
		if (this->length < 32)
		{
			if (wp <= '9') wp -= '0';
			else wp -= ('A' - 10);
			WORD idx = this->inputPos;
			for (WORD i = this->length; i > idx; i--)
			{
				this->code[i] = this->code[i - 1];
			}
			this->code[idx] = wp;
			this->inputPos++;
			this->length++;
			Text_update(this);
			((Component *) this)->repaint = 1;
			InvalidateRect(window, 0, 0);
		}
	}
	else if (wp == 0x08) // VK_BACK
	{
		if (this->inputPos)
		{
			for (WORD i = this->inputPos; i < this->length; i++)
			{
				this->code[i - 1] = this->code[i];
			}
			this->inputPos--;
			this->length--;
			this->code[this->length] = 0;
			Text_update(this);
			((Component *) this)->repaint = 1;
			InvalidateRect(window, 0, 0);
		}
	}
	else if (wp == 0x25) // VK_LEFT
	{
		if (this->inputPos)
		{
			this->inputPos--;
			((Component *) this)->repaint = 1;
			InvalidateRect(window, 0, 0);
		}
	}
	else if (wp == 0x27) // VK_RIGHT
	{
		if (this->inputPos < this->length)
		{
			this->inputPos++;
			((Component *) this)->repaint = 1;
			InvalidateRect(window, 0, 0);
		}
	}
}
SIZE Text_position(Text *this, void *context, SIZE *client)
{
	SIZE pos = {(long) (client->cx - this->width) / 2, 30};
	return pos;
}
SIZE Text_size(Text *this, void *context)
{
	SIZE size = {(long) this->width, (long) this->height};
	return size;
}
void Text_update(Text *this)
{
	BYTE buf[16] = {0};
	for (WORD i = 0; i < 16; i++)
	{
		buf[i] = (this->code[i * 2 + 0] << 4) | this->code[i * 2 + 1];
	}
	mca_decode(&this->view->inst, X64, (char *) buf, 0);
	((Component *) this->view)->repaint = 1;
}