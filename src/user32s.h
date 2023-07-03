#ifndef USER32S_H
#define USER32S_H

typedef struct tagWNDCLASSEXA
{
	unsigned int cbSize;
	unsigned int style;
	void *lpfnWndProc;
	int cbClsExtra;
	int cbWndExtra;
	void *hInstance;
	void *hIcon;
	void *hCursor;
	void *hbrBackground;
	const char *lpszMenuName;
	const char *lpszClassName;
	void *hIconSm;
} WNDCLASSEXA;
typedef struct tagPOINT
{
	long x;
	long y;
} POINT;
typedef struct tagMSG
{
	void               *hwnd;
	unsigned int        message;
	unsigned long long  wParam;
	long long           lParam;
	unsigned long       time;
	POINT               pt;
} MSG;
typedef struct tagSIZE
{
	long cx;
	long cy;
} SIZE, *PSIZE, *LPSIZE;
typedef struct tagRECT
{
	long    left;
	long    top;
	long    right;
	long    bottom;
} RECT;
typedef struct tagPAINTSTRUCT
{
	void          *hdc;
	int            fErase;
	RECT           rcPaint;
	int            fRestore;
	int            fIncUpdate;
	unsigned char  rgbReserved[32];
} PAINTSTRUCT;
typedef struct tagLOGBRUSH
{
	unsigned int lbStyle;
	unsigned long lbColor;
	unsigned long long lbHatch;
} LOGBRUSH;

#endif //USER32S_H
