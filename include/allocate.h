#ifndef ALLOCATE_H
#define ALLOCATE_H

#include <memoryspec.h>

#ifdef __cplusplus
extern "C"
{
#endif

MEMORYAPI void *__cdecl malloc(unsigned long long);
MEMORYAPI void *__cdecl realloc(void *, unsigned long long);
MEMORYAPI void __cdecl free(void *);

MEMORYAPI void *memcpy(void *, const void *, unsigned long long);
MEMORYAPI void *memset(void *, int, unsigned long long);

#ifdef __cplusplus
}
#endif

#endif //ALLOCATE_H
