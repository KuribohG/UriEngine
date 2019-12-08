#pragma once

#include "stdafx.h"

namespace UriEngine
{
	class IGameApp
	{
	public:
		virtual void Init(WNDPROC WndProc) = 0;
		virtual void Update() = 0;
		virtual void Run(WNDPROC WndProc);
		virtual void Finalize();
	};
}

