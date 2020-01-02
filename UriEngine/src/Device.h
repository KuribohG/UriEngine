#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>

namespace UriEngine
{
	extern ID3D12Device2 *g_pDevice;

	class CDevice
	{
	public:
		static CDevice &GetInstance();
		ID3D12Device2 *GetDevice();
	private:
		CDevice();
		~CDevice();
		CDevice(CDevice const&) = delete;
		void operator=(CDevice const&) = delete;

		ID3D12Device2 *m_pDevice;
		IDXGIFactory4 *m_pDxgiFactory;
		IDXGIAdapter4 *m_pDxgiAdapter;
	};
}
