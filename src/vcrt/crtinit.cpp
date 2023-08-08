/*++

Abstract:

    CRT support :: crt init / done functions

Revision History:

    07/08/2023 - Maxim Lyadvinsky - Created

--*/
#include "vcrt/crtinit.h"

#ifdef VSYS_LINKERNEL

typedef void (*Desctructor)(void*);

//using size_t = unsigned long; // __kernel_size_t
//using gfp_t = unsigned int;

extern "C" {
void *__kmalloc(size_t size, gfp_t flags); // NOLINT(bugprone-reserved-identifier)
void kfree(const void *);
}

#include <new>
namespace std { // NOLINT(cert-dcl58-cpp)
    [[maybe_unused]] const std::nothrow_t nothrow {};
    enum class align_val_t : std::size_t {};
}

void operator delete(void *pointer) noexcept {
    kfree(pointer);
}
void operator delete(void *pointer, const std::nothrow_t &) noexcept {
    kfree(pointer);
}
void operator delete(void *pointer, const std::align_val_t) noexcept {
    kfree(pointer);
}
void operator delete(void *pointer, const std::align_val_t, const std::nothrow_t &) noexcept {
    kfree(pointer);
}
void operator delete[](void *pointer, const std::nothrow_t &) noexcept {
    kfree(pointer);
}
void operator delete[](void *pointer) noexcept {
    kfree(pointer);
}
void *__dso_handle {};

extern "C"
int
__cxa_atexit(Desctructor f, void *p, void *d)
{
    return 0;
}

bool
vsys::InitCrt()
{
    return true;
}

void
vsys::DoneCrt()
{

}

#endif // VSYS_LINKERNEL
