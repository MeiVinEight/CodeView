#include "CodeViewWindow.h"

int range(long ax, long ay, long zx, long zy, long x, long y)
{
	return (x >= ax) && (x < zx) && (y >= ay) && (y < zy);
}