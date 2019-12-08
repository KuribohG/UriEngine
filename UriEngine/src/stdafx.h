#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#if defined(CreateWindow)
#undef CreateWindow
#endif

#if defined(max)
#undef max
#endif

#if defined(min)
#undef min
#endif

#include <wrl.h>
using namespace Microsoft::WRL;