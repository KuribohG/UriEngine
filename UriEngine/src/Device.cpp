#include "Device.h"
#include "Utils.h"

namespace UriEngine
{
	ID3D12Device2 *g_pDevice;
	
	CDevice::CDevice()
	{
		bool enableDebugLayer = true;

		UINT createFactoryFlags = 0;
		if (enableDebugLayer)
			createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
		ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&m_pDxgiFactory)));
		if (enableDebugLayer)
		{
			ID3D12Debug *debugInterface;
			ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
			debugInterface->EnableDebugLayer();
		}

		ComPtr<IDXGIAdapter1> dxgiAdapter;
		ComPtr<IDXGIAdapter4> dxgiAdapter4;
		SIZE_T maxVideoMemory = 0;
		for (UINT i = 0; m_pDxgiFactory->EnumAdapters1(i, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
			dxgiAdapter->GetDesc1(&dxgiAdapterDesc);

			if ((dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
				SUCCEEDED(D3D12CreateDevice(dxgiAdapter.Get(),
					D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
				dxgiAdapterDesc.DedicatedVideoMemory > maxVideoMemory)
			{
				maxVideoMemory = dxgiAdapterDesc.DedicatedVideoMemory;
				ThrowIfFailed(dxgiAdapter.As(&dxgiAdapter4));
			}
		}
		m_pDxgiAdapter = dxgiAdapter4.Get();

		ComPtr<ID3D12Device2> pDevice;
		ThrowIfFailed(D3D12CreateDevice(m_pDxgiAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pDevice)));
		m_pDevice = pDevice.Get();

		if (enableDebugLayer)
		{
			ComPtr<ID3D12InfoQueue> pInfoQueue;
			if (SUCCEEDED(pDevice.As(&pInfoQueue)))
			{
				pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
				pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
				pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

				D3D12_MESSAGE_SEVERITY Severities[] =
				{
					D3D12_MESSAGE_SEVERITY_INFO
				};

				// Suppress individual messages by their ID
				D3D12_MESSAGE_ID DenyIds[] = {
					D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
					D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
					D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
				};


				D3D12_INFO_QUEUE_FILTER NewFilter = {};
				//NewFilter.DenyList.NumCategories = _countof(Categories);
				//NewFilter.DenyList.pCategoryList = Categories;
				NewFilter.DenyList.NumSeverities = _countof(Severities);
				NewFilter.DenyList.pSeverityList = Severities;
				NewFilter.DenyList.NumIDs = _countof(DenyIds);
				NewFilter.DenyList.pIDList = DenyIds;

				ThrowIfFailed(pInfoQueue->PushStorageFilter(&NewFilter));
			}
		}
	}

	CDevice& CDevice::GetInstance()
	{
		static CDevice instance;
		return instance;
	}
}