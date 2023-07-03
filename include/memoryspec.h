#ifndef MEMORYAPI_H
#define MEMORYAPI_H

#ifdef MEMORY_SHARED
#define MEMORYAPI __declspec(dllexport)
#else
#define MEMORYAPI __declspec(dllimport)
#endif

#endif //MEMORYAPI_H
