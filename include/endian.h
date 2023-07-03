#ifndef ENDIAN_H
#define ENDIAN_H

#include <memoryspec.h>
#include <WinType.h>

namespace Memory
{
	namespace BE
	{
		MEMORYAPI QWORD get(const void *, BYTE);
		MEMORYAPI WORD get(WORD);
		MEMORYAPI DWORD get(DWORD);
		MEMORYAPI QWORD get(QWORD);
		MEMORYAPI void set(QWORD, void *, BYTE);
	}
	namespace LE
	{
		MEMORYAPI QWORD get(const void *, BYTE);
		MEMORYAPI WORD get(WORD);
		MEMORYAPI DWORD get(DWORD);
		MEMORYAPI QWORD get(QWORD);
		MEMORYAPI void set(QWORD, void *, BYTE);
	}
}

#endif //ENDIAN_H
