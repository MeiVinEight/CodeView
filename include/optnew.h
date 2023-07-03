#ifndef OPTNEW_H
#define OPTNEW_H


void *__cdecl operator new(unsigned long long);
void *__cdecl operator new[](unsigned long long);
void *__cdecl operator new(unsigned long long, void *) noexcept;
void __cdecl operator delete(void *) noexcept;
void __cdecl operator delete(void *, unsigned long long) noexcept;
void __cdecl operator delete[](void *, unsigned long long) noexcept;
void __cdecl operator delete(void *, void *) noexcept;


#endif //OPTNEW_H
