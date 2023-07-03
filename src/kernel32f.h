#ifndef KERNEL32_H
#define KERNEL32_H

#include <WinType.h>

typedef struct STARTUPINFOW
{
	unsigned long         cb;
	const unsigned short *lpReserved;
	const unsigned short *lpDesktop;
	const unsigned short *lpTitle;
	unsigned long         dwX;
	unsigned long         dwY;
	unsigned long         dwXSize;
	unsigned long         dwYSize;
	unsigned long         dwXCountChars;
	unsigned long         dwYCountChars;
	unsigned long         dwFillAttribute;
	unsigned long         dwFlags;
	unsigned short        wShowWindow;
	unsigned short        cbReserved2;
	unsigned char        *lpReserved2;
	void                 *hStdInput;
	void                 *hStdOutput;
	void                 *hStdError;
} STARTUPINFOW;

DWORD __stdcall GetLastError();
void  __stdcall GetStartupInfoW(STARTUPINFOW *);
void *__stdcall GetStdHandle(DWORD);
int   __stdcall WriteConsoleA(void *, const void *, DWORD, void *, void *);

#endif //KERNEL32_H
