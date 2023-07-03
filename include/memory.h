#ifndef MEMORY_MEMORY_H
#define MEMORY_MEMORY_H

#include <memoryspec.h>
#include <WinType.h>


namespace Memory
{
	static const BYTE NTSTATUS = 0;
	static const BYTE DOSERROR = 1;
	static const BYTE EXTERNAL = 2;

	class string
	{
		public:
		QWORD length = 0;
		BYTE *address = nullptr;

		MEMORYAPI string();
		MEMORYAPI explicit string(QWORD);
		MEMORYAPI string(const Memory::string &);
		MEMORYAPI string(Memory::string &&) noexcept;
		MEMORYAPI ~string();
		MEMORYAPI Memory::string &operator=(const Memory::string &);
		MEMORYAPI Memory::string &operator=(Memory::string &&) noexcept;
		MEMORYAPI BYTE &operator[](QWORD) const;
		MEMORYAPI void resize(QWORD);
	};

	MEMORYAPI extern const BYTE BENDIAN;
	MEMORYAPI extern const BYTE LENDIAN;
	MEMORYAPI extern const DWORD ERRNO_SUCCESS;
	MEMORYAPI extern const DWORD ERRNO_ACCESS_VIOLATION;
	MEMORYAPI extern const DWORD ERRNO_OBJECT_CLOSED;

	MEMORYAPI void *allocate(QWORD);
	MEMORYAPI void *reallocate(void *, QWORD);
	MEMORYAPI void free(void *);
	MEMORYAPI void copy(void *, const void *, QWORD);
	MEMORYAPI void fill(void *, BYTE, QWORD);
	MEMORYAPI bool compare(const void *, const void *, QWORD);
	MEMORYAPI void reverse(void *, QWORD);
	MEMORYAPI void violation();
}

#endif //MEMORY_MEMORY_H
