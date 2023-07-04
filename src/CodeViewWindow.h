#ifndef CODEVIEWWINDOW_H
#define CODEVIEWWINDOW_H

#include "component.h"

typedef struct CodeViewWindow
{
	long x;
	long y;
	Component *focus;
	Component *hover;
} CodeViewWindow;

int range(long, long, long, long, long, long);

#endif //CODEVIEWWINDOW_H
