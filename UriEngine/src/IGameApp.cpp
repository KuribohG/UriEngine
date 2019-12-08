#include "IGameApp.h"

namespace UriEngine
{
	void IGameApp::Run(WNDPROC WndProc)
	{
		Init(WndProc);
	}

	void IGameApp::Finalize()
	{

	}
}