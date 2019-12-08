#pragma once

#include "stdafx.h"

namespace UriEngine
{
	class CMainWnd
	{
	public:
		int CreateWindow(WNDPROC WndProc);
		HWND GetHWnd();
	private:
		HWND m_hWnd;
	};
}