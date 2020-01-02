#include "Resource.h"
#include "Device.h"
#include "Utils.h"

namespace UriEngine
{
	void CResource::CreateCommitted(const D3D12_HEAP_PROPERTIES *pHeapProperties, D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC *pDesc, D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE *pOptimizedClearValue)
	{
		ThrowIfFailed(g_pDevice->CreateCommittedResource(
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
}
