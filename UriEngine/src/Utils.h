#pragma once

#include "stdafx.h"
#include <exception>

#define SAFE_DELETE(p) { if (p) { delete (p); (p) = NULL; } }
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = NULL; } }

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw std::exception();
	}
}