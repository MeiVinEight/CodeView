#ifndef GDI32_H
#define GDI32_H

#include <WinType.h>

int   __stdcall BitBlt(void *, int, int, int, int, void *, int, int, DWORD);
int   __stdcall CombineRgn(void *, void *, void *, int);
void *__stdcall CreateCompatibleBitmap(void *, int, int);
void *__stdcall CreateCompatibleDC(void *);
void *__stdcall CreateFontA(int, int, int, int, int, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, const char *);
void *__stdcall CreatePen(int, int, DWORD);
void *__stdcall CreateRectRgn(int x1, int y1, int x2, int y2);
void *__stdcall CreateRoundRectRgn(int, int, int, int, int, int);
void *__stdcall CreateSolidBrush(unsigned long);
int   __stdcall DeleteDC(void *);
int   __stdcall DeleteObject(void *);
void *__stdcall ExtCreatePen(DWORD, DWORD, const void *, DWORD, DWORD *);
int   __stdcall FrameRgn(void *, void *, void *, int, int);
void *__stdcall GetStockObject(int);
int   __stdcall GetTextExtentPoint32A(void *, const char *, int, void *);
int   __stdcall LineTo(void *, int, int);
int   __stdcall MoveToEx(void *, int, int, void *);
int   __stdcall PaintRgn(void *, void *);
int   __stdcall PtInRegion(void *, int, int);
void *__stdcall SelectObject(void *, void *);
DWORD __stdcall SetBkColor(void *, DWORD);
int   __stdcall SetBkMode(void *, int);
int   __stdcall SwapBuffers(void *);
int   __stdcall TextOutA(void *, int, int, const char *, int);

#endif //GDI32_H
