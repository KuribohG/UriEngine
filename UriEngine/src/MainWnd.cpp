#include "MainWnd.h"
#include <tchar.h>

namespace UriEngine
{
	int CMainWnd::CreateWindow(WNDPROC WndProc)
	{
		const TCHAR szWindowClass[] = _T("UriEngine Main Window Class");
		const TCHAR szTitle[] = _T("UriEngine");
		HINSTANCE hInstance = GetModuleHandle(nullptr);

		WNDCLASSEX wcex = {};
		
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex))
		{
			MessageBox(NULL,
				_T("Call to RegisterClassEx failed!"),
				_T("Windows Desktop Guided Tour"),
				NULL);

			return 1;
		}

		m_hWnd = CreateWindowEx(
			WS_EX_LEFT,
			szWindowClass,
			szTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			1920, 1080,
			NULL,
			NULL,
			hInstance,
			NULL
		);

		if (!m_hWnd)
		{
			MessageBox(NULL,
				_T("Call to CreateWindowEx failed!"),
				_T("Windows Desktop Guided Tour"),
				NULL);

			return 1;
		}
	}

	HWND CMainWnd::GetHWnd()
	{
		return m_hWnd;
	}
}