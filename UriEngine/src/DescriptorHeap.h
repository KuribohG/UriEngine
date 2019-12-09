#pragma once

#include "stdafx.h"
#include <d3d12.h>

namespace UriEngine
{
	class CDescriptorHeap
	{
	public:
		CDescriptorHeap();
		~CDescriptorHeap();
		void Create(ID3D12Device *pDevice, const D3D12_DESCRIPTOR_HEAP_DESC &desc);
	private:
		ID3D12DescriptorHeap *m_pDescriptorHeap;
	};
}