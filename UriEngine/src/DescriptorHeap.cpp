#include "Utils.h"
#include "DescriptorHeap.h"

namespace UriEngine
{
	CDescriptorHeap::CDescriptorHeap()
	{
		m_pDescriptorHeap = nullptr;
	}

	CDescriptorHeap::~CDescriptorHeap()
	{
		SAFE_RELEASE(m_pDescriptorHeap);
	}

	void CDescriptorHeap::Create(ID3D12Device *pDevice, const D3D12_DESCRIPTOR_HEAP_DESC& desc)
	{
		pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pDescriptorHeap));
	}
}