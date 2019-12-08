#include <Windows.h>
#include "IGameApp.h"
#include "Render.h"
#include "MainWnd.h"
#include <cstdio>

#define RUN(app) app->Run(WndProc)

using namespace std;
using namespace UriEngine;

class CGameApp : public IGameApp
{
public:
	void Init(WNDPROC WndProc)
	{
		m_bInitialized = false;
		CMainWnd mainWnd;
		mainWnd.CreateWindow(WndProc);
		HWND hwnd = mainWnd.GetHWnd();

		render = new CRender;
		render->CreateRenderWindow(hwnd);
		m_bInitialized = true;
		ShowWindow(hwnd, SW_SHOWNORMAL);

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Update()
	{
		render->BeginFrame();
		float clearColorValue[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		render->ClearRenderTargetView(clearColorValue);
		render->Present();
	}

	bool Initialized()
	{
		return m_bInitialized;
	}
private:
	CRender *render;
	bool m_bInitialized;
};

CGameApp *app;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (app->Initialized())
	{
		switch (message)
		{
		case WM_PAINT:
			app->Update();
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	if (AllocConsole()) {
		freopen("CONOUT$", "w", stdout);
		SetConsoleTitle("Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}

	app = new CGameApp;
	RUN(app);

	return 0;
}