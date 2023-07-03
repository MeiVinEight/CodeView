#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <memoryspec.h>
#include <WinType.h>
#include <memory.h>

namespace Memory
{
	class exception
	{
		public:
		class frame
		{
			public:
			void *address = nullptr;
			void *offset = nullptr;
			void *module = nullptr;
			Memory::string function;
			Memory::string library;

			frame() = delete;
			MEMORYAPI frame(void *);
			MEMORYAPI frame(const Memory::exception::frame &);
			MEMORYAPI ~frame();
		};

		DWORD code = 0;
		BYTE type = Memory::DOSERROR;
		Memory::string message;
		Memory::exception::frame *stack = nullptr;
		DWORD count = 0;
		MEMORYAPI exception(DWORD, BYTE);
		MEMORYAPI exception(DWORD);
		MEMORYAPI exception(const Memory::exception &) noexcept;
		MEMORYAPI ~exception();
	};
	MEMORYAPI DWORD registry(const char *);
	MEMORYAPI Memory::string message(DWORD, BYTE);
	MEMORYAPI DWORD error();
	MEMORYAPI void error(DWORD);
}

#endif //EXCEPTION_H
