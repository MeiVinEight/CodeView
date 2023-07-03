#ifndef USER32_H
#define USER32_H

#include <WinType.h>

#include "user32s.h"

void     *__stdcall BeginPaint(void *, void *);
void     *__stdcall CreateWindowExA(DWORD, const char *, const char *, DWORD, DWORD, DWORD, DWORD, DWORD, void *, void *, void *, void *);
long long __stdcall DefWindowProcA(void *, unsigned int, unsigned long long, long long);
long long __stdcall DispatchMessageA(const void *);
int       __stdcall DrawTextExA(void *, char *, int, void *, unsigned int, void *);
int       __stdcall EndPaint(void *, const void *);
int       __stdcall FillRect(void *, void *, void *);
int       __stdcall GetClientRect(void *, void *);
void     *__stdcall GetDC(void *);
int       __stdcall GetMessageA(void *, void *, unsigned int, unsigned int);
QWORD     __stdcall GetWindowLongPtrA(void *, int);
int       __stdcall InvalidateRect(void *, void *, int);
int       __stdcall InvalidateRgn(void *, void *, int);
void     *__stdcall LoadCursorA(void *, const char *);
void     *__stdcall LoadIconA(void *, const char *);
void      __stdcall PostQuitMessage(int);
int       __stdcall PtInRect(void *, POINT);
int       __stdcall RedrawWindow(void *, void *, void *, unsigned int);
WORD      __stdcall RegisterClassExA(const void *);
int       __stdcall ReleaseDC(void *, void *);
void     *__stdcall SetCursor(void *);
QWORD     __stdcall SetWindowLongPtrA(void *, int, QWORD);
int       __stdcall ShowWindow(void *, int);
int       __stdcall TranslateMessage(const void *);
int       __stdcall UpdateWindow(void *);
int       __stdcall ValidateRect(void *, const void *);
int       __stdcall ValidateRgn(void *, const void *);

#endif //USER32_H
