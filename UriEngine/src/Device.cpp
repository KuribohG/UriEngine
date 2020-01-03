#include "Device.h"
#include "Utils.h"
#include "Config.h"

namespace UriEngine
{	
	CDevice::CDevice()
	{
		bool enableDebugLayer = CConfig::GetInstance().GetEnableDebugLayer();

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

		IDXGIAdapter1 *dxgiAdapter;
		SIZE_T maxVideoMemory = 0;
		for (UINT i = 0; m_pDxgiFactory->EnumAdapters1(i, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC1 dxgiAdapterDesc;
			dxgiAdapter->GetDesc1(&dxgiAdapterDesc);

			if ((dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
				SUCCEEDED(D3D12CreateDevice(dxgiAdapter,
					D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
				dxgiAdapterDesc.DedicatedVideoMemory > maxVideoMemory)
			{
				maxVideoMemory = dxgiAdapterDesc.DedicatedVideoMemory;
				ThrowIfFailed(dxgiAdapter->QueryInterface(IID_PPV_ARGS(&m_pDxgiAdapter)));
			}
		}

		ThrowIfFailed(D3D12CreateDevice(m_pDxgiAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice)));

		if (enableDebugLayer)
		{
			ID3D12InfoQueue *pInfoQueue;
			if (SUCCEEDED(m_pDevice->QueryInterface(IID_PPV_ARGS(&pInfoQueue))))
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

	CDevice::~CDevice()
	{
		//SAFE_DELETE(m_pDevice);
		//SAFE_DELETE(m_pDxgiFactory);
		//SAFE_DELETE(m_pDxgiAdapter);
	}

	CDevice& CDevice::GetInstance()
	{
		static CDevice instance;
		return instance;
	}
}