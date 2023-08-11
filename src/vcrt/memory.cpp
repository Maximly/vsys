/*++

Abstract:

    CRT support :: memory functions

Revision History:

    07/08/2023 - Maxim Lyadvinsky - Created

--*/
#ifdef VSYS_LINKERNEL

#include <new>

void*
operator new(const std::size_t size)
{
    return kmalloc(size, GFP_KERNEL);
}

void*
operator new(const std::size_t size, const std::nothrow_t&) noexcept
{
    return kmalloc(size, GFP_KERNEL);
}

void*
operator new[](const std::size_t size)
{
    return kmalloc(size, GFP_KERNEL);
}

void*
operator new[](const std::size_t size, const std::nothrow_t &) noexcept
{
    return kmalloc(size, GFP_KERNEL);
}

void
operator delete(void *p) noexcept
{
    kfree(p);
}

void
operator delete(void *p, const std::nothrow_t &) noexcept
{
    kfree(p);
}

void
operator delete[](void* p, const std::nothrow_t &) noexcept
{
    kfree(p);
}

void operator delete[](void *p) noexcept
{
    kfree(p);
}

#endif // VSYS_LINKERNEL
