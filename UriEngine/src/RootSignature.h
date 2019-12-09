#pragma once

#include "stdafx.h"
#include <d3d12.h>

namespace UriEngine
{
	class CRootSignature
	{
	public:
		CRootSignature();
		~CRootSignature();
	private:
		ID3D12RootSignature *m_pRootSignature;
	};
}