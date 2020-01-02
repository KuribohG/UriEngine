#pragma once

#include "stdafx.h"
#include "d3d12.h"

namespace UriEngine
{
	class CResource
	{
	public:
		void CreateCommitted(const D3D12_HEAP_PROPERTIES *pHeapProperties, D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC *pDesc, D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE *pOptimizedClearValue);
		ID3D12Resource *GetResource();
		D3D12_RESOURCE_STATES GetCurrentState();
	private:
		ID3D12Resource *m_pResource;
		D3D12_RESOURCE_STATES m_CurrentState;
	};
}