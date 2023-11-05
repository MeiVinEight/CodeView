#include <allocate.h>

#include "WindowMessage.h"
#include "WindowStyles.h"
#include "Code.h"
#include "user32f.h"
#include "gdi32f.h"
#include "Text.h"
#include "CodeViewWindow.h"
#include "CodeUnit.h"

extern unsigned short __ImageBase;

typedef struct ListNode
{
	void *data;
	void *next;
} ListNode;

ListNode *element = 0;

void AddComponent(Component *component)
{
	ListNode *node = (ListNode *) malloc(sizeof(ListNode));
	node->data = component;
	node->next = element;
	element = node;
}
void FreeList(ListNode **list, void (*data)(void *))
{
	ListNode *node = *list;
	while (node)
	{
		ListNode *next = node->next;
		Component *component = (Component *) node->data;
		component->_1(component);
		data(component);
		free(node);
		node = next;
	}
	*list = 0;
}
long long __stdcall WindowProc(void *hwnd, unsigned int uMSG, unsigned long long wParam, long long lParam)
{
	switch (uMSG)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps = {0};
			void *hdc = BeginPaint(hwnd, &ps);
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			SIZE client = {clientRect.right, clientRect.bottom};

			void *context = CreateCompatibleDC(hdc);

			ListNode *elmt = element;
			while (elmt)
			{
				Component *cmp = (Component *) elmt->data;
				if (cmp->repaint)
				{
					SIZE size = cmp->size(cmp, context);
					void *bitmap = SelectObject(context, CreateCompatibleBitmap(hdc, size.cx, size.cy));
					void *brush = CreateSolidBrush(0x00FFFFFF);
					RECT full = {0, 0, size.cx, size.cy};
					FillRect(context, &full, brush);
					DeleteObject(brush);
					cmp->paint(cmp, hwnd, context);
					SIZE pos = cmp->position(cmp, context, &client);
					void *thisRange = CreateRectRgn(pos.cx, pos.cy, pos.cx + size.cx, pos.cy + size.cy);
					if (cmp->previous)
					{
						CombineRgn(cmp->previous, cmp->previous, thisRange, 4); // RGN_DIFF
						void *bk = SelectObject(hdc, CreateSolidBrush(0x00FFFFFF));
						PaintRgn(hdc, cmp->previous);
						DeleteObject(SelectObject(hdc, bk));
						DeleteObject(cmp->previous);
					}
					cmp->previous = thisRange;
					BitBlt(hdc, pos.cx, pos.cy, size.cx, size.cy, context, 0, 0, 0x00CC0020);
					bitmap = SelectObject(context, bitmap);
					DeleteObject(bitmap);
				}
				elmt = elmt->next;
			}

			DeleteDC(context);

			EndPaint(hwnd, &ps);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			WORD x = (lParam >> 0x00) & 0xFFFF;
			WORD y = (lParam >> 0x10) & 0xFFFF;

			void *context = GetDC(hwnd);

			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			SIZE client = {clientRect.right, clientRect.bottom};

			ListNode *elmt = element;
			while (elmt)
			{
				Component *cmp = (Component *) elmt->data;
				cmp->move(cmp, hwnd, context, &client, x, y);
				elmt = elmt->next;
			}
			ReleaseDC(hwnd, context);
			CodeViewWindow *cvw = (CodeViewWindow *) GetWindowLongPtrA(hwnd, 0);
			cvw->x = x;
			cvw->y = y;

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			WORD x = (lParam >> 0x00) & 0xFFFF;
			WORD y = (lParam >> 0x10) & 0xFFFF;

			void *context = GetDC(hwnd);

			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			SIZE client = {clientRect.right, clientRect.bottom};

			ListNode *elmt = element;
			int click = 0;
			while (elmt)
			{
				Component *cmp = (Component *) elmt->data;
				SIZE pos = cmp->position(cmp, context, &client);
				SIZE size = cmp->size(cmp, context);
				if (range(pos.cx, pos.cy, pos.cx + size.cx, pos.cy + size.cy, x, y))
				{
					click = 1;
					cmp->click(cmp, hwnd);
				}
				elmt = elmt->next;
			}
			CodeViewWindow *cvw = (CodeViewWindow *) GetWindowLongPtrA(hwnd, 0);
			if (!click && cvw->focus)
			{
				cvw->focus->repaint = 1;
				cvw->focus = 0;
				InvalidateRect(hwnd, 0, 0);
			}
			ReleaseDC(hwnd, context);
			return 0;
		}
		case WM_KEYDOWN:
		{
			CodeViewWindow *cvw = (CodeViewWindow *) GetWindowLongPtrA(hwnd, 0);
			ListNode *node = element;
			while (node)
			{
				Component *cmp = (Component *) node->data;
				if (cvw->focus == cmp)
				{
					cmp->keyboard(cmp, hwnd, wParam, lParam);
				}
				node = node->next;
			}
			return 0;
		}
	}
	return DefWindowProcA(hwnd, uMSG, wParam, lParam);
}
int WinMain(void *ists, void *prv, char *cmd, int show)
{
	const char className[] = "CodeView";
	WNDCLASSEXA wc = {0};
	wc.style = 0;
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = ists;
	wc.lpszClassName = className;
	wc.hIcon = LoadIconA(ists, "IDI_ICON1");
	wc.hbrBackground = GetStockObject(0);
	// wc.hCursor = LoadCursorA(0, (void *) 32512);
	wc.cbWndExtra = 8;
	RegisterClassExA(&wc);

	unsigned long exsty = 0;
	unsigned long style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	void *hwnd = CreateWindowExA(exsty, className, className, style, 0x80000000, 0x80000000, 974, 547, 0, 0, ists, 0);
	if (!hwnd)
	{
		return 0;
	}
	CodeViewWindow *cvw = (CodeViewWindow *) malloc(sizeof(CodeViewWindow));
	cvw->x = 0;
	cvw->y = 0;
	cvw->focus = 0;
	cvw->hover = 0;
	SetWindowLongPtrA(hwnd, 0, (QWORD) cvw);

	// void *text = CreateWindowExA(0, "EDIT", 0, ES_UPPERCASE, 30, 30, 0xE0, 0x1E, hwnd, 0, ists, 0);
	// DWORD err = GetLastError();
	ShowWindow(hwnd, show);

	Code *code = (Code *) malloc(sizeof(Code));
	CodeUnit *unit = (CodeUnit *) malloc(sizeof(CodeUnit));
	Text *text = (Text *) malloc(sizeof(Text));

	Code__0(code, (Component *) unit);
	CodeUnit__0(unit, code);
	Text__0(text, code);

	AddComponent((Component *) text);
	AddComponent((Component *) unit);
	AddComponent((Component *) code);

	MSG msg = {0};
	while (GetMessageA(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	FreeList(&element, free);
	return 0;
}

#include "kernel32f.h"
#include "ShowWindowFlag.h"

void WinMainCRTStartup(void)
{
	STARTUPINFOW si = {0};
	GetStartupInfoW(&si);
	unsigned short show = si.dwFlags & 0x00000001 ? si.wShowWindow : SW_SHOWDEFAULT;
	WinMain(&__ImageBase, 0, 0, show);
}
void mainCRTStartup(void)
{
	WinMainCRTStartup();
}