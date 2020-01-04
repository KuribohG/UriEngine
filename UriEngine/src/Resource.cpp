#include "Resource.h"
#include "Device.h"
#include "Utils.h"

namespace UriEngine
{
	CResource::CResource()
	{
		m_pDevice = CDevice::GetInstance().GetDevice();
		m_CurrentState = D3D12_RESOURCE_STATE_COMMON;
		m_pResource = nullptr;
	}

	CResource::~CResource()
	{
		//SAFE_DELETE(m_pResource);
	}

	void CResource::CreateCommitted(const D3D12_HEAP_PROPERTIES *pHeapProperties, D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC *pDesc, D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE *pOptimizedClearValue)
	{
		ThrowIfFailed(m_pDevice->CreateCommittedResource(
			pHeapProperties,
			HeapFlags,
			pDesc,
			InitialResourceState,
			pOptimizedClearValue,
			IID_PPV_ARGS(&m_pResource)));
	}

	ID3D12Resource *CResource::GetResource()
	{
		return m_pResource;
	}

	D3D12_RESOURCE_STATES CResource::GetCurrentState()
	{
		return m_CurrentState;
	}

	D3D12_GPU_VIRTUAL_ADDRESS CResource::GetGPUVirtualAddress()
	{
		return m_pResource->GetGPUVirtualAddress();
	}
}
