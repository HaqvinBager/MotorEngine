#pragma once

#include <corecrt.h>

_CRT_BEGIN_C_HEADER

#ifndef _RETAIL
_ACRTIMP void __cdecl _wassert(
    _In_z_ wchar_t const* _Message,
    _In_z_ wchar_t const* _File,
    _In_   unsigned       _Line
);

    #define dl_assert(expression) (void)(                                                           \
                (!!(expression)) ||                                                              \
                (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \
            )
#endif

_CRT_END_C_HEADER
